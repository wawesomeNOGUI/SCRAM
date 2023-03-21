--
-- Control Logic Unit
-- Actually Has Most of SCRAM in here except for the timer
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
--use ieee.numeric_bit.all;

ENTITY clu IS
	PORT (
		CLK, ClkEn, RST : IN BIT;
		
		--set to 1 to clr timer after cycle is done
		tCLR	: OUT BIT;			
		
		PC : BUFFER UNSIGNED(7 DOWNTO 0);
		MAR : BUFFER UNSIGNED(7 DOWNTO 0);		
		AC : BUFFER UNSIGNED(7 DOWNTO 0);
	
		q : BUFFER UNSIGNED(3 DOWNTO 0);	
		
		t : IN BIT_VECTOR(3 DOWNTO 0));
END clu;

ARCHITECTURE Behavioral OF clu IS
	type t_ram	is array (0 to 15) of UNSIGNED(7 DOWNTO 0);
--RAM
	--SIGNAL RAM : t_ram := (("00010101"), ("01100100"), ("00110101"), ("01110000"), ("00000001"), ("00001111"), others=>"00000000");
	--Decrement and rollover back to three program:
	SIGNAL RAM : t_ram := (("00011001"), ("01101010"), ("00111001"), ("10000101"), ("01110000"), ("00011011"), ("00111001"), ("01110000"), ("00000000"), ("00000011"), ("00000001"), ("00000011"), others=>"00000000");
--Registers
	--SIGNAL PC : UNSIGNED( 7 DOWNTO 0);
	SIGNAL IR : UNSIGNED( 7 DOWNTO 0);
	--SIGNAL MAR : UNSIGNED( 7 DOWNTO 0);
	SIGNAL MBR : UNSIGNED( 7 DOWNTO 0);
	--SIGNAL AC : UNSIGNED( 7 DOWNTO 0);
	SIGNAL AD : UNSIGNED( 7 DOWNTO 0);
--Instruction Code Control
	--SIGNAL q : UNSIGNED( 3 DOWNTO 0);

BEGIN
--Constant connections
	q <= IR(7 DOWNTO 4); --directly connect q to IR(C) to get opcode 
	
PROCESS
BEGIN
	WAIT UNTIL CLK'event AND CLK = '1'; --rising edge	
	
	IF RST = '1' THEN
		PC <= "00000000";
		IR <= "00000000";
		MAR <= "00000000";
		MBR <= "00000000";
		AC <= "00000000";
		--AD <= "00000000";
		tCLR <= '1';
	END IF;
	
	IF ClkEn = '1' THEN
		tCLR <= '0'; --reset timer clr
		CASE t IS
		--
		--=================FETCH====================
		--
			WHEN "0000" =>
				MAR <= PC;
			WHEN "0001" => 
				MBR <= RAM(to_integer(MAR));
				PC <= PC + 1;
			WHEN "0010" =>
				IR <= MBR;
		--
		--================EXECUTE===================
		--
			WHEN "0011" => 
				CASE q IS
					WHEN "0001" =>	--LDA
						MAR <= resize(IR(3 DOWNTO 0), MAR'length); -- get operand
					WHEN "0010" => --LDI
						MAR <= resize(IR(3 DOWNTO 0), MAR'length);
					WHEN "0011" =>	--STA
						MAR <= resize(IR(3 DOWNTO 0), MAR'length);
					WHEN "0101" => --ADD
						MAR <= resize(IR(3 DOWNTO 0), MAR'length);
					WHEN "0110" => --SUB
						MAR <= resize(IR(3 DOWNTO 0), MAR'length);
					WHEN "0111" => --JMP END
						PC <= resize(IR(3 DOWNTO 0), MAR'length);
						tCLR <= '1';
					WHEN "1000" => --JMZ
						IF AC = "00000000" THEN
							PC <= resize(IR(3 DOWNTO 0), MAR'length);
							tCLR <= '1';
						ELSE 
							tCLR <= '1';
						END IF;
					WHEN OTHERS => NULL;
				END CASE;
			WHEN "0100" => 
				CASE q IS
					WHEN "0001" => --LDA
						MBR <= RAM(to_integer(MAR));
					WHEN "0010" => --LDI
						MBR <= RAM(to_integer(MAR));
					WHEN "0011" =>	--STA END
						RAM(to_integer(MAR)) <= AC;
						tCLR <= '1';
					WHEN "0101" => --ADD
						MBR <= RAM(to_integer(MAR));
					WHEN "0110" => --SUB
						MBR <= RAM(to_integer(MAR));
					WHEN OTHERS => NULL;
				END CASE;
			WHEN "0101" => 
				CASE q IS
					WHEN "0001" => --LDA END
						AC <= MBR;
						tCLR <= '1';
					WHEN "0010" => --LDI
						MAR <= MBR;
					WHEN "0101" => --ADD
						AD <= MBR;
					WHEN "0110" => --SUB
						AD <= MBR;
					WHEN OTHERS => NULL;
				END CASE;
			WHEN "0110" =>
				CASE q IS
					WHEN "0010" => --LDI
						MBR <= RAM(to_integer(MAR));
					WHEN "0101" => --ADD
						AD <= AD + AC;
					WHEN "0110" => --SUB
						AD <= NOT AD + 1;
					WHEN OTHERS => NULL;
				END CASE;
			WHEN "0111" =>
				CASE q IS
					WHEN "0010" => --LDI END
						AC <= MBR;
						tCLR <= '1';
					WHEN "0101" => --ADD END
						AC <= AD;
						tCLR <= '1';
					WHEN "0110" => --SUB
						AD <= AD + AC;
					WHEN OTHERS => NULL;
				END CASE;
			WHEN "1000" =>
				CASE q IS
					WHEN "0110" => --SUB END
						AC <= AD;
						tCLR <= '1';
					WHEN OTHERS => NULL;					
				END CASE;
			--WHEN "1001" => t <= "0000"; Cout <= '1';
			WHEN OTHERS => NULL;
		END CASE;
	 END IF;
END PROCESS;
END Behavioral;