program Program9;
var i, x, y, c: integer;
  r1, r2: real;
begin
  x:=1;
  for i:=1 to 5 do x:=x*3;
  writeln(x);
  y:=1;
  for i:=5 downto 1 do y:=y*3;
  writeln(y);
  for i:=x to y do writeln(99);
  r1:=0.5;
  r2:=3.4;
  for i:=x mod 10 to x div 10 do begin
    r1:=r1+r1*r2;
    r2:=r2+0.1;
  end;
  writeln(r1);
  writeln(r2);
end.
