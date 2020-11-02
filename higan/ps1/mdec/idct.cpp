// 1D-IDCT x2
//   f'(u,y) = C(v,y) * F (u,v)
//   f (x,y) = C(u,x) * f'(u,y)
//     where C = 1/2 * transposed_scaleTable
auto MDEC::IDCT(i16 (*f)[8], i16* F) -> void {
  i16 f_[8][8];

  for(uint y : range(8)) {
    for(uint u : range(8)) {
      i32 sum = 0;
      for(uint v : range(8)) {
        sum += scaleTable[y][v] * F[v * 8 + u];
      }
      // given scaleTable is 16-bit signed integer,
      // so convert to floating point with round,
      // and result is multiplied by 1/2 to calculate the C
      f_[y][u] = (sum + 16384) >> 15 >> 1;
    }
  }
  for(uint y : range(8)) {
    for(uint x : range(8)) {
      i32 sum = 0;
      for(uint u : range(8)) {
        sum += scaleTable[x][u] * f_[y][u];
      }
      f[y][x] = sclamp<8>((sum + 16384) >> 15 >> 1);
    }
  }
}
