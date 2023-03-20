--
-- Frquency Divider
--
ENTITY div IS
	PORT (
		ClkEn, CLK   : IN BIT;  --Takes system clock in
		Cout 	  		 : OUT BIT); 
END div;

ARCHITECTURE Behavioral OF div IS
SIGNAL count : INTEGER RANGE 0 TO 16777215;--67108863;	--FREQ_DIV
BEGIN
PROCESS
BEGIN
	WAIT UNTIL CLK'event AND CLK = '1'; --rising edge
	IF CLKEn = '1' THEN
		IF count > 0 THEN --FREQ DIV
			count <= count + 1;
			Cout <= '0';
		ELSE
			count <= count + 1;
			Cout <= '1';
		END IF;
	END IF;
END PROCESS;
END Behavioral;