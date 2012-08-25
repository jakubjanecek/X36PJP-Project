program Program6;
var x, y: integer;
begin
  x:=154;
  y:=99;
  if x < y then y:=y-x else x:=x-y;
  writeln(x);
  writeln(y);
  if x > y then writeln(1) else writeln(2);
  if x <= y then writeln(3) else writeln(4);
  if x = y then writeln(5);
  if x <> y then writeln(6);
  x:=100; y:=200;
  if x > y then x:=x-1 else if x < y then y:=y-1;
  writeln(x);
  writeln(y);
  if 2*x >= 2*y+1 then writeln(x*x) else writeln(y*y);
end.
