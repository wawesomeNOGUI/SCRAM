# SCRAM
The Simple but Complete Random Access Machine as described in "The (New) Turing Omnibus" book implemented in VHDL.
The SCRAM is a 4-bit computer that uses 8-bit words. Instructions are encoded with the leading 4 bits being the opcode
and the trailing 4 bits the operand. This effectively means a program can't reference more than 4 bits in memory manipulation or arithmetic. 
![SCRAM](https://user-images.githubusercontent.com/67801725/227410379-2a481daa-8d7f-4dc1-9bbd-ecedf4382d19.png)
