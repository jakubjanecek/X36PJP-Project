program Program7;
var v1, v2: real;
begin
  v1:=12.67;
  v2:=67.12;
  if v1 < v2 then v2:=v2-v1 else v1:=v1-v2;
  writeln(v1);
  writeln(v2);
  if v1 > v2 then begin
    writeln(1);
    v1:=v1+0.5;
  end else begin
    writeln(2);
    v2:=v2+0.5;
  end;
  if v1 <= v2 then writeln(3) else writeln(4);
  if v1 = v2 then writeln(5);
  if v1 <> v2 then begin
    writeln(6);
    v1:=v2*2;
  end;
  v1:=99.99; v2:=888.88;
  if v1 > v2 then v1:=v1-1.22 else if v1 < v2 then v2:=v2-1.44;
  writeln(v1);
  writeln(v2);
  if v1*2 >= v2*2+1 then writeln(v1*v1) else writeln(v2/v1);
end.
