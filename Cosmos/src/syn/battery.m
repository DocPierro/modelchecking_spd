syms y1(t) y2(t) y10 y20 c k L y(t) A b F kc kmc;
y = [y1; y2];

A = [-kc kmc; kc -kmc];
b = [-L; 0];
F = A*y+b;

zt = dsolve(diff(y)==F, y(0)==[y10;y20]);

zt.y2
