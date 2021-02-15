EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 1
Title "LED Controller"
Date "2021-02-14"
Rev "V1"
Comp "Quantum Empbedded Systems"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x03_Male J4
U 1 1 602A7478
P 6750 3600
F 0 "J4" H 6850 3800 50  0000 L CNN
F 1 "Serial Connect" H 6650 3350 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6750 3600 50  0001 C CNN
F 3 "~" H 6750 3600 50  0001 C CNN
	1    6750 3600
	-1   0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 602A7CC6
P 1050 2400
F 0 "J1" H 1050 2150 50  0000 C CNN
F 1 "5V Power" H 1050 2550 50  0000 C CNN
F 2 "Connector_Wire:SolderWire-2.5sqmm_1x02_P8.8mm_D2.4mm_OD4.4mm" H 1050 2400 50  0001 C CNN
F 3 "~" H 1050 2400 50  0001 C CNN
	1    1050 2400
	-1   0    0    1   
$EndComp
$Comp
L Device:C C1
U 1 1 602ABE5F
P 1600 2450
F 0 "C1" H 1715 2496 50  0000 L CNN
F 1 "1 uF" H 1715 2405 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 1638 2300 50  0001 C CNN
F 3 "~" H 1600 2450 50  0001 C CNN
	1    1600 2450
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_PIC18:PIC18F26K22-xSO U1
U 1 1 602AEAF8
P 4450 3550
F 0 "U1" H 3950 4600 50  0000 C CNN
F 1 "PIC18F27Q43-xSO" H 4900 4600 50  0000 C CNN
F 2 "Package_SO:SOIC-28W_7.5x17.9mm_P1.27mm" H 4650 3550 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/40001412G.pdf" H 4650 3550 50  0001 C CNN
	1    4450 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 602AFB39
P 4800 2000
F 0 "C2" V 4915 2046 50  0000 L CNN
F 1 "0.1 uF" V 4600 1950 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 4838 1850 50  0001 C CNN
F 3 "~" H 4800 2000 50  0001 C CNN
	1    4800 2000
	0    1    1    0   
$EndComp
$Comp
L power:VDD #PWR01
U 1 1 602B01BA
P 1600 2100
F 0 "#PWR01" H 1600 1950 50  0001 C CNN
F 1 "VDD" H 1615 2273 50  0000 C CNN
F 2 "" H 1600 2100 50  0001 C CNN
F 3 "" H 1600 2100 50  0001 C CNN
	1    1600 2100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 602B0C64
P 1600 2750
F 0 "#PWR02" H 1600 2500 50  0001 C CNN
F 1 "GND" H 1605 2577 50  0000 C CNN
F 2 "" H 1600 2750 50  0001 C CNN
F 3 "" H 1600 2750 50  0001 C CNN
	1    1600 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 602B1183
P 5250 2100
F 0 "#PWR06" H 5250 1850 50  0001 C CNN
F 1 "GND" H 5255 1927 50  0000 C CNN
F 2 "" H 5250 2100 50  0001 C CNN
F 3 "" H 5250 2100 50  0001 C CNN
	1    5250 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 2300 1600 2100
Wire Wire Line
	1250 2300 1600 2300
Connection ~ 1600 2300
Wire Wire Line
	1600 2750 1600 2650
Wire Wire Line
	1250 2400 1400 2400
Wire Wire Line
	1400 2400 1400 2650
Wire Wire Line
	1400 2650 1600 2650
Connection ~ 1600 2650
Wire Wire Line
	1600 2650 1600 2600
$Comp
L Connector:Conn_01x03_Male J9
U 1 1 602AAFA4
P 9150 3800
F 0 "J9" H 9122 3732 50  0000 R CNN
F 1 "String5" H 9122 3823 50  0000 R CNN
F 2 "Connector_Wire:SolderWire-1sqmm_1x03_P7.8mm_D1.4mm_OD3.9mm" H 9150 3800 50  0001 C CNN
F 3 "~" H 9150 3800 50  0001 C CNN
	1    9150 3800
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male J8
U 1 1 602AA6DA
P 9150 3250
F 0 "J8" H 9122 3182 50  0000 R CNN
F 1 "String4" H 9122 3273 50  0000 R CNN
F 2 "Connector_Wire:SolderWire-1sqmm_1x03_P7.8mm_D1.4mm_OD3.9mm" H 9150 3250 50  0001 C CNN
F 3 "~" H 9150 3250 50  0001 C CNN
	1    9150 3250
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male J7
U 1 1 602A9B25
P 9150 2700
F 0 "J7" H 9122 2632 50  0000 R CNN
F 1 "String3" H 9122 2723 50  0000 R CNN
F 2 "Connector_Wire:SolderWire-1sqmm_1x03_P7.8mm_D1.4mm_OD3.9mm" H 9150 2700 50  0001 C CNN
F 3 "~" H 9150 2700 50  0001 C CNN
	1    9150 2700
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male J6
U 1 1 602A9004
P 9150 2200
F 0 "J6" H 9122 2132 50  0000 R CNN
F 1 "String2" H 9122 2223 50  0000 R CNN
F 2 "Connector_Wire:SolderWire-1sqmm_1x03_P7.8mm_D1.4mm_OD3.9mm" H 9150 2200 50  0001 C CNN
F 3 "~" H 9150 2200 50  0001 C CNN
	1    9150 2200
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male J5
U 1 1 602A882C
P 9150 1700
F 0 "J5" H 9122 1632 50  0000 R CNN
F 1 "String1" H 9122 1723 50  0000 R CNN
F 2 "Connector_Wire:SolderWire-1sqmm_1x03_P7.8mm_D1.4mm_OD3.9mm" H 9150 1700 50  0001 C CNN
F 3 "~" H 9150 1700 50  0001 C CNN
	1    9150 1700
	-1   0    0    1   
$EndComp
$Comp
L power:VDD #PWR010
U 1 1 602C05A1
P 8450 1200
F 0 "#PWR010" H 8450 1050 50  0001 C CNN
F 1 "VDD" H 8465 1373 50  0000 C CNN
F 2 "" H 8450 1200 50  0001 C CNN
F 3 "" H 8450 1200 50  0001 C CNN
	1    8450 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 602C0B28
P 8700 4800
F 0 "#PWR011" H 8700 4550 50  0001 C CNN
F 1 "GND" H 8705 4627 50  0000 C CNN
F 2 "" H 8700 4800 50  0001 C CNN
F 3 "" H 8700 4800 50  0001 C CNN
	1    8700 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 3900 8950 3900
Wire Wire Line
	8950 3350 8700 3350
Wire Wire Line
	8700 3350 8700 3900
Connection ~ 8700 3900
Wire Wire Line
	8950 2800 8700 2800
Wire Wire Line
	8700 2800 8700 3350
Connection ~ 8700 3350
Wire Wire Line
	8950 2300 8700 2300
Wire Wire Line
	8700 2300 8700 2800
Connection ~ 8700 2800
Wire Wire Line
	8950 1800 8700 1800
Wire Wire Line
	8700 1800 8700 2300
Connection ~ 8700 2300
Wire Wire Line
	8950 1600 8450 1600
Wire Wire Line
	8950 2100 8450 2100
Wire Wire Line
	8450 2100 8450 1600
Connection ~ 8450 1600
Wire Wire Line
	8950 2600 8450 2600
Wire Wire Line
	8450 2600 8450 2100
Connection ~ 8450 2100
Wire Wire Line
	8950 3150 8450 3150
Wire Wire Line
	8450 3150 8450 2600
Connection ~ 8450 2600
Wire Wire Line
	8950 3700 8450 3700
Wire Wire Line
	8450 3700 8450 3150
Connection ~ 8450 3150
$Comp
L Connector:Conn_01x05_Male J2
U 1 1 602D318D
P 5800 5650
F 0 "J2" V 5908 6031 50  0000 C CNN
F 1 "ICSP" V 5700 5650 50  0000 C CNN
F 2 "" H 5800 5650 50  0001 C CNN
F 3 "~" H 5800 5650 50  0001 C CNN
	1    5800 5650
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x03_Male J3
U 1 1 6029D4DE
P 6600 4600
F 0 "J3" H 6650 4800 50  0000 L CNN
F 1 "RCin" H 6350 4600 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 6600 4600 50  0001 C CNN
F 3 "~" H 6600 4600 50  0001 C CNN
	1    6600 4600
	-1   0    0    1   
$EndComp
Wire Wire Line
	8450 1200 8450 1600
$Comp
L Device:R R2
U 1 1 6029F916
P 3300 2250
F 0 "R2" H 3370 2296 50  0000 L CNN
F 1 "10k" H 3370 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3230 2250 50  0001 C CNN
F 3 "~" H 3300 2250 50  0001 C CNN
	1    3300 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2950 3300 2950
Wire Wire Line
	3300 2950 3300 2400
$Comp
L power:VDD #PWR04
U 1 1 602A05DD
P 4450 1900
F 0 "#PWR04" H 4450 1750 50  0001 C CNN
F 1 "VDD" H 4465 2073 50  0000 C CNN
F 2 "" H 4450 1900 50  0001 C CNN
F 3 "" H 4450 1900 50  0001 C CNN
	1    4450 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 2100 3300 2000
Wire Wire Line
	3300 2000 4450 2000
Wire Wire Line
	4450 1900 4450 2000
Connection ~ 4450 2000
Wire Wire Line
	4450 2000 4450 2550
$Comp
L power:GND #PWR05
U 1 1 602A2E68
P 4450 4850
F 0 "#PWR05" H 4450 4600 50  0001 C CNN
F 1 "GND" H 4455 4677 50  0000 C CNN
F 2 "" H 4450 4850 50  0001 C CNN
F 3 "" H 4450 4850 50  0001 C CNN
	1    4450 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 4850 4450 4650
Wire Wire Line
	5250 2100 5250 2000
Wire Wire Line
	5250 2000 4950 2000
Wire Wire Line
	4650 2000 4450 2000
Wire Wire Line
	3300 2950 3300 5300
Wire Wire Line
	3300 5300 5600 5300
Wire Wire Line
	5600 5300 5600 5450
Connection ~ 3300 2950
Wire Wire Line
	5800 5450 5800 4650
Wire Wire Line
	5800 4650 4450 4650
Connection ~ 4450 4650
Wire Wire Line
	4450 4650 4450 4550
$Comp
L power:VDD #PWR07
U 1 1 602AD71B
P 5700 5100
F 0 "#PWR07" H 5700 4950 50  0001 C CNN
F 1 "VDD" H 5715 5273 50  0000 C CNN
F 2 "" H 5700 5100 50  0001 C CNN
F 3 "" H 5700 5100 50  0001 C CNN
	1    5700 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 5450 5700 5100
Wire Wire Line
	5150 3450 5900 3450
Wire Wire Line
	5900 3450 5900 5450
Wire Wire Line
	5150 3350 6000 3350
Wire Wire Line
	6000 3350 6000 5450
Wire Wire Line
	3750 3450 2900 3450
Wire Wire Line
	2900 3450 2900 1600
Wire Wire Line
	2900 1600 7950 1600
Wire Wire Line
	7950 1600 7950 1700
Wire Wire Line
	7950 1700 8950 1700
Wire Wire Line
	3750 3550 2750 3550
Wire Wire Line
	2750 3550 2750 1500
Wire Wire Line
	2750 1500 8150 1500
Wire Wire Line
	8150 1500 8150 2200
Wire Wire Line
	8150 2200 8950 2200
Wire Wire Line
	5150 3050 7650 3050
Wire Wire Line
	7650 3050 7650 2700
Wire Wire Line
	7650 2700 8950 2700
Wire Wire Line
	5150 2950 7150 2950
Wire Wire Line
	7150 2950 7150 3250
Wire Wire Line
	7150 3250 8950 3250
Wire Wire Line
	5150 4150 7900 4150
Wire Wire Line
	7900 4150 7900 3800
Wire Wire Line
	7900 3800 8950 3800
Wire Wire Line
	5150 3750 6450 3750
Wire Wire Line
	6450 3750 6450 3700
Wire Wire Line
	6450 3700 6550 3700
Wire Wire Line
	5150 3650 6200 3650
Wire Wire Line
	6200 3650 6200 3500
Wire Wire Line
	6200 3500 6550 3500
$Comp
L power:GND #PWR09
U 1 1 602BC9A3
P 6300 3600
F 0 "#PWR09" H 6300 3350 50  0001 C CNN
F 1 "GND" H 6200 3500 50  0000 C CNN
F 2 "" H 6300 3600 50  0001 C CNN
F 3 "" H 6300 3600 50  0001 C CNN
	1    6300 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3600 6300 3600
Wire Wire Line
	5150 3850 6200 3850
Wire Wire Line
	6200 3850 6200 4500
Wire Wire Line
	6200 4500 6400 4500
$Comp
L power:GND #PWR08
U 1 1 602C1BEB
P 6250 4950
F 0 "#PWR08" H 6250 4700 50  0001 C CNN
F 1 "GND" H 6255 4777 50  0000 C CNN
F 2 "" H 6250 4950 50  0001 C CNN
F 3 "" H 6250 4950 50  0001 C CNN
	1    6250 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 4700 6250 4700
Wire Wire Line
	6250 4700 6250 4950
$Comp
L Connector:Conn_01x03_Male J10
U 1 1 602ADC6B
P 9150 4400
F 0 "J10" H 9122 4332 50  0000 R CNN
F 1 "String6" H 9122 4423 50  0000 R CNN
F 2 "Connector_Wire:SolderWire-1sqmm_1x03_P7.8mm_D1.4mm_OD3.9mm" H 9150 4400 50  0001 C CNN
F 3 "~" H 9150 4400 50  0001 C CNN
	1    9150 4400
	-1   0    0    1   
$EndComp
Wire Wire Line
	8700 3900 8700 4500
Wire Wire Line
	8950 4500 8700 4500
Connection ~ 8700 4500
Wire Wire Line
	8700 4500 8700 4800
Wire Wire Line
	8950 4300 8450 4300
Wire Wire Line
	8450 4300 8450 3700
Connection ~ 8450 3700
Wire Wire Line
	8950 4400 7400 4400
Wire Wire Line
	7400 4400 7400 4250
Wire Wire Line
	7400 4250 5150 4250
$Comp
L Device:LED D1
U 1 1 602B6ED3
P 2550 4050
F 0 "D1" H 2543 4267 50  0000 C CNN
F 1 "LED" H 2543 4176 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2550 4050 50  0001 C CNN
F 3 "~" H 2550 4050 50  0001 C CNN
	1    2550 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 602B7BC0
P 2050 4250
F 0 "R1" H 2120 4296 50  0000 L CNN
F 1 "220" H 2120 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1980 4250 50  0001 C CNN
F 3 "~" H 2050 4250 50  0001 C CNN
	1    2050 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 602B7F4F
P 2050 4550
F 0 "#PWR03" H 2050 4300 50  0001 C CNN
F 1 "GND" H 2055 4377 50  0000 C CNN
F 2 "" H 2050 4550 50  0001 C CNN
F 3 "" H 2050 4550 50  0001 C CNN
	1    2050 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 4050 2700 4050
Wire Wire Line
	2050 4550 2050 4400
Wire Wire Line
	2050 4100 2050 4050
Wire Wire Line
	2050 4050 2400 4050
Text Notes 8950 1400 0    50   ~ 0
LED Strings
$EndSCHEMATC
