--
-- Timer T is a 10-bit up counter to control the CLU
--
ENTITY timer IS
	PORT (
		CLK, ClkEn, CLR  : IN BIT;
		t 	  			     : BUFFER BIT_VECTOR(3 DOWNTO 0);
		Cout 			     : OUT BIT); 
END timer;

ARCHITECTURE Behavioral OF timer IS
BEGIN
PROCESS
BEGIN
	WAIT UNTIL CLK'event AND CLK = '1'; --rising edge
		IF ClkEn = '1' THEN --count up on buffer t
			CASE t IS
				WHEN "0000" => t <= "0001"; Cout <= '0';
				WHEN "0001" => t <= "0010"; Cout <= '0';
				WHEN "0010" => t <= "0011"; Cout <= '0';
				WHEN "0011" => t <= "0100"; Cout <= '0';
				WHEN "0100" => t <= "0101"; Cout <= '0';
				WHEN "0101" => t <= "0110"; Cout <= '0';
				WHEN "0110" => t <= "0111"; Cout <= '0';
				WHEN "0111" => t <= "1000"; Cout <= '0';
				WHEN "1000" => t <= "1001"; Cout <= '0';
				WHEN "1001" => t <= "0000"; Cout <= '1';
				WHEN OTHERS => t <= "0000"; Cout <= '1';
			END CASE;
		ELSE
			t <=t;
			Cout <= '0';

		END IF;
		
		IF CLR = '1' THEN
			t <= "0000";
			Cout <= '1';
		END IF;
END PROCESS;
END Behavioral;