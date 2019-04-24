//Sanyo LC89510

auto MCD::CDC::clock() -> void {
  if(stopwatch) stopwatch--;
}

auto MCD::CDC::read() -> uint8 {
//print("CDC ", hex(address), "\n");

  uint8 data;

  switch(address++) {

  case 0x0: {  //COMIN
    data = comin;
  } break;

  case 0x1: {  //IFSTAT
    data.bit(0) = ifstat.sten;
    data.bit(1) = ifstat.dten;
    data.bit(2) = ifstat.stbsy;
    data.bit(3) = ifstat.dtbsy;
    data.bit(4) = 1;
    data.bit(5) = ifstat.deci;
    data.bit(6) = ifstat.dtei;
    data.bit(7) = ifstat.cmdi;
  } break;

  case 0x2: {  //DBCL
    data = dbc.byte(0);
  } break;

  case 0x3: {  //DBCH
    data = dbc.byte(1);
  } break;

  case 0x4: {  //HEAD0
    data = head[ctrl1.shdren][0];
  } break;

  case 0x5: {  //HEAD1
    data = head[ctrl1.shdren][1];
  } break;

  case 0x6: {  //HEAD2
    data = head[ctrl1.shdren][2];
  } break;

  case 0x7: {  //HEAD3
    data = head[ctrl1.shdren][3];
  } break;

  case 0x8: {  //PTL
    data = pt.byte(0);
  } break;

  case 0x9: {  //PTH
    data = pt.byte(1);
  } break;

  case 0xa: {  //WAL
    data = wa.byte(0);
  } break;

  case 0xb: {  //WAH
    data = wa.byte(1);
  } break;

  case 0xc: {  //STAT0
    data.bit(0) = stat0.uceblk;
    data.bit(1) = stat0.erablk;
    data.bit(2) = stat0.sblk;
    data.bit(3) = stat0.wshort;
    data.bit(4) = stat0.lblk;
    data.bit(5) = stat0.nosync;
    data.bit(6) = stat0.ilsync;
    data.bit(7) = stat0.crcok;
  } break;

  case 0xd: {  //STAT1
    data.bit(0) = stat1.sh3era;
    data.bit(1) = stat1.sh2era;
    data.bit(2) = stat1.sh1era;
    data.bit(3) = stat1.sh0era;
    data.bit(4) = stat1.modera;
    data.bit(5) = stat1.blkera;
    data.bit(6) = stat1.secera;
    data.bit(7) = stat1.menera;
  } break;

  case 0xe: {  //STAT2
    data.bit(0) = stat2.rform0;
    data.bit(1) = stat2.rform1;
    data.bit(2) = stat2.nocor;
    data.bit(3) = stat2.mode;
    data.bit(4) = stat2.rmod0;
    data.bit(5) = stat2.rmod1;
    data.bit(6) = stat2.rmod2;
    data.bit(7) = stat2.rmod3;
  } break;

  case 0xf: {  //STAT3
    data.bit(5) = stat3.cblk;
    data.bit(6) = stat3.wlong;
    data.bit(7) = stat3.valst | 1;  //hack: always mark /VALST as clear
    ifstat.deci |= 1;  //clear pending decoder interrupt
  } break;

  }

  return data;
}

auto MCD::CDC::write(uint8 data) -> void {
//print("CDC ", hex(address), "=", hex(data), "\n");
  switch(address++) {

  case 0x0: {  //SBOUT
    sbout = data;
  } break;

  case 0x1: {  //IFCTRL
    ifctrl.souten = data.bit(0);
    ifctrl.douten = data.bit(1);
    ifctrl.stwai  = data.bit(2);
    ifctrl.dtwai  = data.bit(3);
    ifctrl.cmdbk  = data.bit(4);
    ifctrl.decien = data.bit(5);
    ifctrl.dteien = data.bit(6);
    ifctrl.cmdien = data.bit(7);

    if((ifctrl.dteien && !ifstat.dtei)
    || (ifctrl.decien && !ifstat.deci)
    ) {
      irq.raise();
    } else if(irq.pending) {
      irq.lower();
    }

    //abort data transfer if data output is disabled
    if(!ifctrl.douten) {
      ifstat.dten = 1;
      ifstat.dtbsy = 1;
    }
  } break;

  case 0x2: {  //DBCL
    dbc.byte(0) = data;
  } break;

  case 0x3: {  //DBCH
    dbc.byte(1) = data;
  } break;

  case 0x4: {  //DACL
    dac.byte(0) = data;
  } break;

  case 0x5: {  //DACH
    dac.byte(1) = data;
  } break;

  case 0x6: {  //DTRG
    if(!ifctrl.douten) break;
    ifstat.dten = 0;
    ifstat.dtbsy = 0;
    dbc = (uint12)dbc;  //clear d12-d15 of DBC
    dsr = 0;
    edt = 0;
    switch(destination) {
    case 2:  //main CPU read
    case 3:  //sub CPU read
      dsr = 1;
      break;
    }
  } break;

  case 0x7: {  //DTACK
    ifstat.dtei |= 1;  //clear end data transfer interrupt flag
    dbc = (uint12)dbc;  //clear d12-d15 of DBC
  } break;

  case 0x8: {  //WAL
    wa.byte(0) = data;
  } break;

  case 0x9: {  //WAH
    wa.byte(1) = data;
  } break;

  case 0xa: {  //CTRL0
    ctrl0.prq    = data.bit(0);
    ctrl0.orq    = data.bit(1);
    ctrl0.wrrq   = data.bit(2);
    ctrl0.eramrq = data.bit(3);
    ctrl0.autorq = data.bit(4);
    ctrl0.e01rq  = data.bit(5);
    ctrl0.decen  = data.bit(7);

    stat0.crcok = ctrl0.decen;  //set CRC OK bit only when decoder is enabled

    //update decoding mode
    stat2.mode = ctrl1.modrq;
    if(!ctrl0.autorq) stat2.nocor = ctrl1.formrq;
  } break;

  case 0xb: {  //CTRL1
    ctrl1.shdren = data.bit(0);
    ctrl1.mbckrq = data.bit(1);
    ctrl1.formrq = data.bit(2);
    ctrl1.modrq  = data.bit(3);
    ctrl1.cowren = data.bit(4);
    ctrl1.dscren = data.bit(5);
    ctrl1.syden  = data.bit(6);
    ctrl1.syen   = data.bit(7);

    //update decoding mode
    stat2.mode = ctrl1.modrq;
    if(!ctrl0.autorq) stat2.nocor = ctrl1.formrq;
  } break;

  case 0xc: {  //PTL
    pt.byte(0) = data;
  } break;

  case 0xd: {  //PTH
    pt.byte(1) = data;
  } break;

  case 0xe: {  //CTRL2
    ctrl2.stentrg = data.bit(0);
    ctrl2.stenctl = data.bit(1);
    ctrl2.eramsl  = data.bit(2);
  } break;

  case 0xf: {  //RESET
    irq.lower();

    comin = 0xff;
    sbout = 0xff;
    head[0][0] = 0;
    head[0][1] = 0;
    head[0][2] = 0;
    head[0][3] = 1;
    head[1][0] = 0;
    head[1][1] = 0;
    head[1][2] = 0;
    head[1][3] = 0;
    ifstat = {};
    ifctrl = {};
  } break;

  }
}

auto MCD::CDC::power(bool reset) -> void {
  dsr = 0;
  edt = 0;
  address = 0;
  destination = 0;

  comin = 0xff;
  sbout = 0xff;
  dbc = 0;
  dac = 0;
  wa = 0;
  pt = 0;
  head[0][0] = 0;
  head[0][1] = 0;
  head[0][2] = 0;
  head[0][3] = 1;
  head[1][0] = 0;
  head[1][1] = 0;
  head[1][2] = 0;
  head[1][3] = 0;

  ifstat = {};
  stat0 = {};
  stat1 = {};
  stat2 = {};
  stat3 = {};
  ifctrl = {};
  ctrl0 = {};
  ctrl1 = {};
  ctrl2 = {};
}
