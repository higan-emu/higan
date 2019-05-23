//Sanyo LC8951x (CD controller)
//note: this class emulates a theoretically perfect CDC that never encounters read errors

//called whenever IRQ line state may change
auto MCD::CDC::poll() -> void {
  bool pending = false;
  pending |= irq.decoder.enable  && irq.decoder.pending;
  pending |= irq.transfer.enable && irq.transfer.pending;
  pending |= irq.command.enable  && irq.command.pending;
if(irq.decoder.enable)irq.decoder.pending=0;
  pending ? irq.raise() : irq.lower();
}

auto MCD::CDC::clock() -> void {
  stopwatch++;
}

auto MCD::CDC::decode(uint sector) -> void {
  if(!decoder.enable || !mcd.fd) return;

  uint minute = sector / 75 / 60 % 60;
  uint second = sector / 75 % 60;
  uint frame  = sector % 75;

  header.minute = minute / 10 << 4 | minute % 10;
  header.second = second / 10 << 4 | second % 10;
  header.frame  = frame  / 10 << 4 | frame  % 10;
  header.mode   = 0x01;

  decoder.valid = 1;
  irq.decoder.pending = 1;
  poll();

  if(control.writeRequest) {
    transfer.pointer += 2352;
    transfer.target  += 2352;

    if(sector >= 150) {
      //the datasheet states that the sync header is written at the tail instead of head.
      //but it seems much more likely what's actually happening is the *next* sector's sync header is written instead,
      //so that is what is being done here.
      mcd.fd->seek((sector - 150) * 2352 + 12);
      for(uint index : range(2352)) {
        ram[(uint14)(transfer.pointer + index)] = mcd.fd->read();
      }
    } else {
      for(uint index : range(2352)) {
        ram[(uint14)(transfer.pointer + index)] = 0x00;
      }
    }
  }
}

auto MCD::CDC::data() -> uint8 {
  if(!transfer.ready) return 0xff;

  uint8 data = ram[(uint14)(transfer.source++)];
  if(!transfer.length--) {  //halt on underflow (-1)
    transfer.active = 0;
    transfer.busy = 0;
    transfer.ready = 0;
    transfer.completed = 1;
    irq.transfer.pending = 1;
    poll();
  }

  return data;
}

auto MCD::CDC::read() -> uint8 {
print("CDC ", hex(address), " ", "\n");

  uint8 data;

  switch(address) {

  //COMIN: command input
  case 0x0: {
    if(command.empty) { data = 0xff; break; }
    data = command.fifo[command.read++];
    if(command.read == command.write) {
      command.empty = 1;
      irq.command.pending = 0;
      poll();
    }
  } break;

  //IFSTAT: interface status
  case 0x1: {
    data.bit(0) =!status.active;
    data.bit(1) =!transfer.active;
    data.bit(2) =!status.busy;
    data.bit(3) =!transfer.busy;
    data.bit(4) = 1;
    data.bit(5) =!irq.decoder.pending;
    data.bit(6) =!irq.transfer.pending;
    data.bit(7) =!irq.command.pending;
  } break;

  //DBCL: data byte counter low
  case 0x2: {
    data.bits(0,7) = transfer.length.bits(0,7);
  } break;

  //DBCH: data byte counter high
  case 0x3: {
    data.bits(0,3) = transfer.length.bits(8,11);
    data.bit (4)   =!irq.transfer.pending;
    data.bit (5)   =!irq.transfer.pending;
    data.bit (6)   =!irq.transfer.pending;
    data.bit (7)   =!irq.transfer.pending;
  } break;

  //HEAD0: header or subheader data
  case 0x4: {
    if(control.head == 0) {
      data = header.minute;
    } else {
      data = subheader.file;
    }
  } break;

  //HEAD1: header or subheader data
  case 0x5: {
    if(control.head == 0) {
      data = header.second;
    } else {
      data = subheader.channel;
    }
  } break;

  //HEAD2: header or subheader data
  case 0x6: {
    if(control.head == 0) {
      data = header.frame;
    } else {
      data = subheader.submode;
    }
  } break;

  //HEAD3: header or subheader data
  case 0x7: {
    if(control.head == 0) {
      data = header.mode;
    } else {
      data = subheader.coding;
    }
  } break;

  //PTL: block pointer low
  case 0x8: {
    data.bits(0,7) = transfer.pointer.bits(0,7);
  } break;

  //PTH: block pointer high
  case 0x9: {
    data.bits(0,7) = transfer.pointer.bits(8,15);
  } break;

  //WAL: write address low
  case 0xa: {
    data.bits(0,7) = transfer.target.bits(0,7);
  } break;

  //WAH: write address high
  case 0xb: {
    data.bits(0,7) = transfer.target.bits(8,15);
  } break;

  //STAT0: status 0
  case 0xc: {
    data.bit(0) = 0;  //UCEBLK: uncorrected errors in block
    data.bit(1) = 0;  //ERABLK: erasures in block
    data.bit(2) = 0;  //SBLK:   short block
    data.bit(3) = 0;  //WSHORT: word short
    data.bit(4) = 0;  //LBLK:   long block
    data.bit(5) = 0;  //NOSYNC: no sync pattern detected
    data.bit(6) = 0;  //ILSYNC: illegal sync
    data.bit(7) = decoder.enable;  //CRCOK: CRC check OK (set only when decoder is enabled)
  } break;

  //STAT1: status 1
  //reports error when reading header or subheader data from blocks
  case 0xd: {
    data.bit(0) = 0;  //SH3ERA: subheader coding read error
    data.bit(1) = 0;  //SH2ERA: subheader submode read error
    data.bit(2) = 0;  //SH1ERA: subheader channel read error
    data.bit(3) = 0;  //SH0ERA: subheader file read error
    data.bit(4) = 0;  //MODERA: header mode read error
    data.bit(5) = 0;  //BLKERA: header block read error
    data.bit(6) = 0;  //SECERA: header second read error
    data.bit(7) = 0;  //MINERA: header minute read error
  } break;

  //STAT2: status 2
  //RFORMx and RMODx are undocumented even across other ICs that emulate the Sanyo interface
  case 0xe: {  //STAT2
    data.bit(0) = 0;             //RFORM0
    data.bit(1) = 0;             //RFORM1
    data.bit(2) = decoder.form;  //FORM (also referred to as NOCOR)
    data.bit(3) = decoder.mode;  //MODE
    data.bit(4) = 0;             //RMOD0
    data.bit(5) = 0;             //RMOD1
    data.bit(6) = 0;             //RMOD2
    data.bit(7) = 0;             //RMOD3
  } break;

  //STAT3: status 3
  case 0xf: {
    data.bit(5) = 0;              //CBLK:  corrected block flag
    data.bit(6) = 0;              //WLONG: word long
    data.bit(7) =!decoder.valid;  //!VALST: valid status
    decoder.valid = 0;            //note: not accurate, supposedly
    irq.decoder.pending = 0;
    poll();
  } break;

  }

  //COMIN reads do not increment the address; STAT3 reads wrap the address to 0x0
  if(address) address++;

  return data;
}

auto MCD::CDC::write(uint8 data) -> void {
print("CDC ", hex(address), "=", hex(data), "\n");
  switch(address) {

  //SBOUT: status byte output
  case 0x0: {
    if(status.wait && transfer.busy) break;
    if(status.read == status.write && !status.empty) status.read++;  //unverified: discard oldest byte?
    status.fifo[status.write++] = data;
    status.empty  = 0;
    status.active = 1;
    status.busy   = 1;
  } break;

  case 0x1: {  //IFCTRL
    status.enable        = data.bit(0);
    transfer.enable      = data.bit(1);
    status.wait          =!data.bit(2);
    transfer.wait        =!data.bit(3);
    control.commandBreak =!data.bit(4);
    irq.decoder.enable   = data.bit(5);
    irq.transfer.enable  = data.bit(6);
    irq.command.enable   = data.bit(7);
    poll();

    //abort data transfer if data output is disabled
    if(!transfer.enable) {
      transfer.active = 0;
      transfer.busy = 0;
    }
  } break;

  //DBCL: data byte counter low
  case 0x2: {
    transfer.length.bits(0,7) = data.bits(0,7);
  } break;

  //DBCH: data byte counter high
  case 0x3: {
    transfer.length.bits(8,11) = data.bits(0,3);
  } break;

  //DACL: data address counter low
  case 0x4: {
    transfer.source.bits(0,7) = data.bits(0,7);
  } break;

  //DACH: data address counter high
  case 0x5: {
    transfer.source.bits(8,15) = data.bits(0,7);
  } break;

  //DTRG: data trigger
  case 0x6: {
    if(!transfer.enable) break;
    transfer.active = 1;
    transfer.busy = 1;
    transfer.ready = 0;
    transfer.completed = 0;
    switch(destination) {
    case 2:  //main CPU read
    case 3:  //sub CPU read
      transfer.ready = 1;
      break;
    }
  } break;

  case 0x7: {  //DTACK
    irq.transfer.pending = 0;
    poll();
  } break;

  //WAL: write address low
  case 0x8: {
    transfer.target.bits(0,7) = data.bits(0,7);
  } break;

  //WAH: write address high
  case 0x9: {
    transfer.target.bits(8,15) = data.bits(0,7);
  } break;

  //CTRL0: control 0
  case 0xa: {
    control.pCodeCorrection = data.bit(0);
    control.qCodeCorrection = data.bit(1);
    control.writeRequest    = data.bit(2);
    control.erasureRequest  = data.bit(3);
    control.autoCorrection  = data.bit(4);
    control.errorCorrection = data.bit(5);
    control.edcCorrection   = data.bit(6);
    decoder.enable          = data.bit(7);

    decoder.mode = control.mode;
    decoder.form = control.form & control.autoCorrection;
  } break;

  //CTRL1: control 1
  case 0xb: {
    control.head            = data.bit(0);
    control.modeByteCheck   = data.bit(1);
    control.form            = data.bit(2);
    control.mode            = data.bit(3);
    control.correctionWrite = data.bit(4);
    control.descramble      = data.bit(5);
    control.syncDetection   = data.bit(6);
    control.syncInterrupt   = data.bit(7);

    decoder.mode = control.mode;
    decoder.form = control.form & control.autoCorrection;
  } break;

  //PTL: block pointer low
  case 0xc: {
    transfer.pointer.bits(0,7) = data.bits(0,7);
  } break;

  //PTH: block pointer high
  case 0xd: {
    transfer.pointer.bits(8,15) = data.bits(0,7);
  } break;

  //CTRL2: control 2
  case 0xe: {
    control.statusTrigger     = data.bit(0);
    control.statusControl     = data.bit(1);
    control.erasureCorrection = data.bit(2);
  } break;

  //RESET: software reset
  case 0xf: {
    irq.lower();

    command = {};
    status = {};
    transfer = {};
    header = {};
    subheader = {};
  } break;

  }

  //SBOUT writes do not increment the address; RESET reads wrap the address to 0x0
  if(address) address++;
}

auto MCD::CDC::power(bool reset) -> void {
  for(auto& byte : ram) byte = 0x00;

  address = 0;
  destination = 0;

  command = {};
  status = {};
  transfer = {};
  decoder = {};
  header = {};
  subheader = {};
}
