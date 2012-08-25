program Program8;
var x, y, c: integer;
  r1, r2: real;
begin
  x:=154;
  y:=8;
  while x <> y do
    if x < y then y:=y-x else x:=x-y;
  writeln(x);
  writeln(y);
  c:=0; x:=99;
  while x > 0 do begin
    c:=c+1;
    x:=x div 2;
  end;
  writeln(c);
  r1:=5.6;
  r2:=78.2;
  while r1 < r2 do begin
    r1:=r1+r1/2-r1/3;
    r2:=r2-r2*0.3;
  end;
  writeln(r1);
  writeln(r2);
end.
