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
P 6900 4550
F 0 "J4" H 7000 4750 50  0000 L CNN
F 1 "Serial Connect" H 6800 4850 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6900 4550 50  0001 C CNN
F 3 "~" H 6900 4550 50  0001 C CNN
	1    6900 4550
	-1   0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 602A7CC6
P 1050 2400
F 0 "J1" H 1050 2150 50  0000 C CNN
F 1 "5V Power" H 1050 2550 50  0000 C CNN
F 2 "Connector_Wire:SolderWire-1sqmm_1x02_P5.4mm_D1.4mm_OD2.7mm" H 1050 2400 50  0001 C CNN
F 3 "~" H 1050 2400 50  0001 C CNN
	1    1050 2400
	-1   0    0    1   
$EndComp
$Comp
L Device:C C1
U 1 1 602ABE5F
P 1600 2450
F 0 "C1" H 1715 2496 50  0000 L CNN
F 1 "10 uF" H 1715 2405 50  0000 L CNN
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
F 1 "PIC18F2xQ43-xSO" H 4900 4600 50  0000 C CNN
F 2 "Package_SO:SOIC-28W_7.5x17.9mm_P1.27mm" H 4650 3550 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/40001412G.pdf" H 4650 3550 50  0001 C CNN
	1    4450 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 602AFB39
P 5000 2300
F 0 "C2" V 5115 2346 50  0000 L CNN
F 1 "0.1 uF" V 4800 2250 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5038 2150 50  0001 C CNN
F 3 "~" H 5000 2300 50  0001 C CNN
	1    5000 2300
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
L power:GND #PWR05
U 1 1 602B1183
P 5450 2400
F 0 "#PWR05" H 5450 2150 50  0001 C CNN
F 1 "GND" H 5455 2227 50  0000 C CNN
F 2 "" H 5450 2400 50  0001 C CNN
F 3 "" H 5450 2400 50  0001 C CNN
	1    5450 2400
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
L power:VDD #PWR09
U 1 1 602C05A1
P 8200 1100
F 0 "#PWR09" H 8200 950 50  0001 C CNN
F 1 "VDD" H 8215 1273 50  0000 C CNN
F 2 "" H 8200 1100 50  0001 C CNN
F 3 "" H 8200 1100 50  0001 C CNN
	1    8200 1100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 602C0B28
P 8350 6150
F 0 "#PWR010" H 8350 5900 50  0001 C CNN
F 1 "GND" H 8355 5977 50  0000 C CNN
F 2 "" H 8350 6150 50  0001 C CNN
F 3 "" H 8350 6150 50  0001 C CNN
	1    8350 6150
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Male J2
U 1 1 602D318D
P 5300 5650
F 0 "J2" V 5408 6031 50  0000 C CNN
F 1 "ICSP" V 5200 5650 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 5300 5650 50  0001 C CNN
F 3 "~" H 5300 5650 50  0001 C CNN
	1    5300 5650
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x03_Male J3
U 1 1 6029D4DE
P 7350 3300
F 0 "J3" H 7400 3500 50  0000 L CNN
F 1 "RCin" H 7100 3300 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 7350 3300 50  0001 C CNN
F 3 "~" H 7350 3300 50  0001 C CNN
	1    7350 3300
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 6029F916
P 3300 2650
F 0 "R1" H 3370 2696 50  0000 L CNN
F 1 "10k" H 3370 2605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3230 2650 50  0001 C CNN
F 3 "~" H 3300 2650 50  0001 C CNN
	1    3300 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2950 3300 2950
Wire Wire Line
	3300 2950 3300 2800
$Comp
L power:VDD #PWR03
U 1 1 602A05DD
P 3350 6000
F 0 "#PWR03" H 3350 5850 50  0001 C CNN
F 1 "VDD" H 3365 6173 50  0000 C CNN
F 2 "" H 3350 6000 50  0001 C CNN
F 3 "" H 3350 6000 50  0001 C CNN
	1    3350 6000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3300 2300 4450 2300
$Comp
L power:GND #PWR04
U 1 1 602A2E68
P 4450 4850
F 0 "#PWR04" H 4450 4600 50  0001 C CNN
F 1 "GND" H 4455 4677 50  0000 C CNN
F 2 "" H 4450 4850 50  0001 C CNN
F 3 "" H 4450 4850 50  0001 C CNN
	1    4450 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 4850 4450 4650
Wire Wire Line
	5450 2400 5450 2300
Wire Wire Line
	5450 2300 5150 2300
Wire Wire Line
	3300 2950 3300 5300
Connection ~ 3300 2950
Connection ~ 4450 4650
Wire Wire Line
	4450 4650 4450 4550
Wire Wire Line
	5150 3450 5900 3450
Wire Wire Line
	5900 3450 5900 4650
Wire Wire Line
	5150 3350 6000 3350
Wire Wire Line
	3750 3450 2900 3450
Wire Wire Line
	2900 3450 2900 1600
Wire Wire Line
	2900 1600 7950 1600
Wire Wire Line
	3750 3550 2750 3550
Wire Wire Line
	2750 3550 2750 1500
$Comp
L power:GND #PWR06
U 1 1 602C1BEB
P 7000 3450
F 0 "#PWR06" H 7000 3200 50  0001 C CNN
F 1 "GND" H 6850 3400 50  0000 C CNN
F 2 "" H 7000 3450 50  0001 C CNN
F 3 "" H 7000 3450 50  0001 C CNN
	1    7000 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 3400 7000 3400
Wire Wire Line
	7000 3400 7000 3450
$Comp
L Device:LED D1
U 1 1 602B6ED3
P 6350 3750
F 0 "D1" H 6343 3967 50  0000 C CNN
F 1 "LED-Red" H 6343 3876 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6350 3750 50  0001 C CNN
F 3 "~" H 6350 3750 50  0001 C CNN
	1    6350 3750
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 602B7BC0
P 6850 3750
F 0 "R2" V 6950 3750 50  0000 L CNN
F 1 "220" V 6750 3700 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6780 3750 50  0001 C CNN
F 3 "~" H 6850 3750 50  0001 C CNN
	1    6850 3750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7000 5400 7000 5700
$Comp
L Jumper:SolderJumper_3_Bridged12 JP1
U 1 1 602C7A81
P 3550 6000
F 0 "JP1" H 3550 6205 50  0000 C CNN
F 1 "uC Power" H 3550 6114 50  0000 C CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged12_RoundedPad1.0x1.5mm_NumberLabels" H 3550 6000 50  0001 C CNN
F 3 "~" H 3550 6000 50  0001 C CNN
	1    3550 6000
	1    0    0    -1  
$EndComp
Connection ~ 4450 2300
Wire Wire Line
	4450 2300 4450 2550
Wire Wire Line
	3300 2300 3300 2500
Wire Wire Line
	4450 2300 4850 2300
Text Label 4450 2000 0    50   ~ 0
uC_Power
Wire Wire Line
	4450 2300 4450 2000
Wire Wire Line
	3750 6000 6100 6000
Wire Wire Line
	5100 5450 5100 5300
Wire Wire Line
	5100 5300 3300 5300
Wire Wire Line
	5300 5450 5300 4650
Wire Wire Line
	5300 4650 4450 4650
Wire Wire Line
	5400 5450 5400 4650
Wire Wire Line
	5400 4650 5900 4650
Wire Wire Line
	5500 5450 5500 4750
Wire Wire Line
	5500 4750 6000 4750
Wire Wire Line
	6000 3350 6000 4750
Text Label 4900 5050 0    50   ~ 0
uC_Power
Wire Wire Line
	5200 5450 5200 5200
Wire Wire Line
	5200 5200 4900 5200
Wire Wire Line
	4900 5200 4900 5050
Text Label 3850 6350 0    50   ~ 0
uC_Power
Wire Wire Line
	3550 6350 3850 6350
Wire Wire Line
	3550 6150 3550 6350
$Comp
L SamacSys_Parts:TSW-102-08-L-T-RA J5
U 1 1 602C605F
P 8600 1850
F 0 "J5" H 9000 2115 50  0000 C CNN
F 1 "TSW-102-08-L-T-RA" H 9000 2024 50  0001 C CNN
F 2 "HDRRA6W64P254_3X2_508X254X810P" H 9250 1950 50  0001 L CNN
F 3 "http://suddendocs.samtec.com/catalog_english/tsw_th.pdf" H 9250 1850 50  0001 L CNN
F 4 "CONN HEADER 6 POS .100\"" H 9250 1750 50  0001 L CNN "Description"
F 5 "8.1" H 9250 1650 50  0001 L CNN "Height"
F 6 "200-TSW10208LTRA" H 9250 1550 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Samtec/TSW-102-08-L-T-RA/?qs=rU5fayqh%252BE1g9ojY3d%2FW1A%3D%3D" H 9250 1450 50  0001 L CNN "Mouser Price/Stock"
F 8 "SAMTEC" H 9250 1350 50  0001 L CNN "Manufacturer_Name"
F 9 "TSW-102-08-L-T-RA" H 9250 1250 50  0001 L CNN "Manufacturer_Part_Number"
	1    8600 1850
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:TSW-102-08-L-T-RA J6
U 1 1 602CA55D
P 8600 3500
F 0 "J6" H 9000 3765 50  0000 C CNN
F 1 "TSW-102-08-L-T-RA" H 9000 3674 50  0001 C CNN
F 2 "HDRRA6W64P254_3X2_508X254X810P" H 9250 3600 50  0001 L CNN
F 3 "http://suddendocs.samtec.com/catalog_english/tsw_th.pdf" H 9250 3500 50  0001 L CNN
F 4 "CONN HEADER 6 POS .100\"" H 9250 3400 50  0001 L CNN "Description"
F 5 "8.1" H 9250 3300 50  0001 L CNN "Height"
F 6 "200-TSW10208LTRA" H 9250 3200 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Samtec/TSW-102-08-L-T-RA/?qs=rU5fayqh%252BE1g9ojY3d%2FW1A%3D%3D" H 9250 3100 50  0001 L CNN "Mouser Price/Stock"
F 8 "SAMTEC" H 9250 3000 50  0001 L CNN "Manufacturer_Name"
F 9 "TSW-102-08-L-T-RA" H 9250 2900 50  0001 L CNN "Manufacturer_Part_Number"
	1    8600 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 5350 9850 3500
Wire Wire Line
	9850 3500 9400 3500
Wire Wire Line
	9400 1850 9850 1850
Wire Wire Line
	9850 1850 9850 3500
Connection ~ 9850 3500
Wire Wire Line
	8200 2050 8200 3700
Wire Wire Line
	8200 2050 8600 2050
Wire Wire Line
	8200 3700 8600 3700
Connection ~ 8200 3700
Wire Wire Line
	8200 3700 8200 5550
$Comp
L SamacSys_Parts:TSW-102-08-L-T-RA J7
U 1 1 602CB4AA
P 8600 5350
F 0 "J7" H 9000 5615 50  0000 C CNN
F 1 "TSW-102-08-L-T-RA" H 9000 5524 50  0001 C CNN
F 2 "HDRRA6W64P254_3X2_508X254X810P" H 9250 5450 50  0001 L CNN
F 3 "http://suddendocs.samtec.com/catalog_english/tsw_th.pdf" H 9250 5350 50  0001 L CNN
F 4 "CONN HEADER 6 POS .100\"" H 9250 5250 50  0001 L CNN "Description"
F 5 "8.1" H 9250 5150 50  0001 L CNN "Height"
F 6 "200-TSW10208LTRA" H 9250 5050 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Samtec/TSW-102-08-L-T-RA/?qs=rU5fayqh%252BE1g9ojY3d%2FW1A%3D%3D" H 9250 4950 50  0001 L CNN "Mouser Price/Stock"
F 8 "SAMTEC" H 9250 4850 50  0001 L CNN "Manufacturer_Name"
F 9 "TSW-102-08-L-T-RA" H 9250 4750 50  0001 L CNN "Manufacturer_Part_Number"
	1    8600 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 5350 9850 5350
Wire Wire Line
	8600 5550 8200 5550
Wire Wire Line
	8600 5450 8350 5450
Wire Wire Line
	8350 5450 8350 5950
Wire Wire Line
	8600 3600 8350 3600
Wire Wire Line
	8350 3600 8350 5450
Connection ~ 8350 5450
Wire Wire Line
	8600 1950 8350 1950
Wire Wire Line
	8350 1950 8350 3600
Connection ~ 8350 3600
Connection ~ 8350 5950
Wire Wire Line
	8350 5950 8350 6150
Wire Wire Line
	9400 3700 9700 3700
Wire Wire Line
	9700 3700 9700 5550
Wire Wire Line
	8350 5950 9700 5950
Wire Wire Line
	9400 5550 9700 5550
Connection ~ 9700 5550
Wire Wire Line
	9700 5550 9700 5950
Wire Wire Line
	9400 2050 9700 2050
Wire Wire Line
	9700 2050 9700 3700
Connection ~ 9700 3700
Wire Wire Line
	8200 1100 8200 1350
Connection ~ 8200 2050
Wire Wire Line
	7950 1850 8600 1850
Wire Wire Line
	7950 1600 7950 1850
Wire Wire Line
	9600 1500 9600 1950
Wire Wire Line
	9600 1950 9400 1950
Wire Wire Line
	2750 1500 9600 1500
Wire Wire Line
	9550 3600 9400 3600
Wire Wire Line
	7400 5350 8600 5350
Wire Wire Line
	9550 5450 9400 5450
Wire Wire Line
	7400 4250 7400 5350
Wire Wire Line
	5150 4250 7400 4250
Wire Wire Line
	9550 4150 9550 5450
Wire Wire Line
	9850 1850 9850 1350
Wire Wire Line
	9850 1350 8200 1350
Connection ~ 9850 1850
Connection ~ 8200 1350
Wire Wire Line
	8200 1350 8200 2050
Wire Wire Line
	5150 3050 7800 3050
Wire Wire Line
	7800 3050 7800 3500
Wire Wire Line
	7800 3500 8600 3500
Wire Wire Line
	5150 2950 9550 2950
Wire Wire Line
	9550 2950 9550 3600
$Comp
L power:GND #PWR08
U 1 1 602B7F4F
P 7200 3800
F 0 "#PWR08" H 7200 3550 50  0001 C CNN
F 1 "GND" H 7205 3627 50  0000 C CNN
F 2 "" H 7200 3800 50  0001 C CNN
F 3 "" H 7200 3800 50  0001 C CNN
	1    7200 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 3750 6700 3750
Wire Wire Line
	7000 3750 7200 3750
Wire Wire Line
	7200 3750 7200 3800
Wire Wire Line
	5150 3650 5700 3650
Wire Wire Line
	5700 3650 5700 3200
Wire Wire Line
	5700 3200 7150 3200
Wire Wire Line
	5150 3750 6200 3750
Wire Wire Line
	6700 4550 6450 4550
$Comp
L power:GND #PWR07
U 1 1 602BC9A3
P 6450 4550
F 0 "#PWR07" H 6450 4300 50  0001 C CNN
F 1 "GND" H 6350 4450 50  0000 C CNN
F 2 "" H 6450 4550 50  0001 C CNN
F 3 "" H 6450 4550 50  0001 C CNN
	1    6450 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3850 5750 3850
Wire Wire Line
	5750 3850 5750 4450
Wire Wire Line
	5750 4450 6700 4450
Wire Wire Line
	5150 4150 9550 4150
Wire Wire Line
	5150 3950 5600 3950
Wire Wire Line
	5600 3950 5600 4550
Wire Wire Line
	5600 4550 6200 4550
Wire Wire Line
	6200 4550 6200 4750
Wire Wire Line
	6200 4750 6650 4750
Wire Wire Line
	6650 4750 6650 4650
Wire Wire Line
	6650 4650 6700 4650
Wire Wire Line
	7150 3300 6100 3300
Wire Wire Line
	6100 3300 6100 6000
Text Notes 6950 4500 0    50   ~ 0
RX
Text Notes 6950 4700 0    50   ~ 0
TX
Text Notes 6950 4600 0    50   ~ 0
GND
Text Notes 3800 6250 0    50   ~ 0
1-2 Power from 5V LED rail (default)\n2-3 Power from RC input
$EndSCHEMATC
