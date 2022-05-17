
; 1. this line is ok
X:   cmp r1,r2

; 2. this line is ok (highest positive value that fits in 12 bits)
Z1:  .data  2047

; 3. data overflow (positive value too high to fit in 12 bits)
Z2:  .data  2048

; 4. this line is ok (lowest negative value that fits in 12 bits)
Z3:  .data  -2048

; 5. data overflow (negative value too low to fit in 12 bits)
Z4:  .data  -2049

; 6. this line is ok
     stop
