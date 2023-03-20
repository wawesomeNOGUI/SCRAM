ENTITY hex7seg IS
	PORT(hex_symbol	: IN BIT_VECTOR (3 DOWNTO 0);
		  segment		: OUT BIT_VECTOR (0 TO 6));
END hex7seg;

ARCHITECTURE behavioral OF hex7seg IS
BEGIN
WITH hex_symbol SELECT
segment <=  "0000001" WHEN "0000",
				"1001111" WHEN "0001",
				"0010010" WHEN "0010",
				"0000110" WHEN "0011",
				"1001100" WHEN "0100",
				"0100100" WHEN "0101",
				"0100000" WHEN "0110",
				"0001111" WHEN "0111",
				"0000000" WHEN "1000",
				"0000100" WHEN "1001",
				"0001000" WHEN "1010",
				"1100000" WHEN "1011",
				"0110001" WHEN "1100",
				"1000010" WHEN "1101",
				"0110000" WHEN "1110",
				"0111000" WHEN "1111";
END behavioral;