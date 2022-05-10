EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Display_Character:KCSC02-123 U1
U 1 1 626C855C
P 2500 1550
F 0 "U1" H 2500 2307 50  0000 C CNN
F 1 "KCSC02-123" H 2500 2216 50  0000 C CNN
F 2 "Display_7Segment:7SegmentLED_LTS6760_LTS6780" H 2500 2125 50  0000 C CNN
F 3 "http://www.kingbright.com/attachments/file/psearch/000/00/00/KCSC02-123(Ver.10A).pdf" H 2000 2025 50  0001 L CNN
	1    2500 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 626CD3F2
P 1600 1250
F 0 "R1" V 1393 1250 50  0000 C CNN
F 1 "330" V 1484 1250 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1530 1250 50  0001 C CNN
F 3 "~" H 1600 1250 50  0001 C CNN
	1    1600 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 626D2FC3
P 1600 1350
F 0 "R2" V 1393 1350 50  0000 C CNN
F 1 "330" V 1484 1350 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1530 1350 50  0001 C CNN
F 3 "~" H 1600 1350 50  0001 C CNN
	1    1600 1350
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 626D33BF
P 1600 1450
F 0 "R3" V 1393 1450 50  0000 C CNN
F 1 "330" V 1484 1450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1530 1450 50  0001 C CNN
F 3 "~" H 1600 1450 50  0001 C CNN
	1    1600 1450
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 626D36D3
P 1600 1550
F 0 "R4" V 1393 1550 50  0000 C CNN
F 1 "330" V 1484 1550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1530 1550 50  0001 C CNN
F 3 "~" H 1600 1550 50  0001 C CNN
	1    1600 1550
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 626D3932
P 1600 1650
F 0 "R5" V 1393 1650 50  0000 C CNN
F 1 "330" V 1484 1650 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1530 1650 50  0001 C CNN
F 3 "~" H 1600 1650 50  0001 C CNN
	1    1600 1650
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 626D3C27
P 1600 1750
F 0 "R6" V 1393 1750 50  0000 C CNN
F 1 "330" V 1484 1750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1530 1750 50  0001 C CNN
F 3 "~" H 1600 1750 50  0001 C CNN
	1    1600 1750
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 626D3EC1
P 1600 1850
F 0 "R7" V 1393 1850 50  0000 C CNN
F 1 "330" V 1484 1850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1530 1850 50  0001 C CNN
F 3 "~" H 1600 1850 50  0001 C CNN
	1    1600 1850
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 626D4188
P 1600 1950
F 0 "R8" V 1393 1950 50  0000 C CNN
F 1 "330" V 1484 1950 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1530 1950 50  0001 C CNN
F 3 "~" H 1600 1950 50  0001 C CNN
	1    1600 1950
	0    1    1    0   
$EndComp
Wire Wire Line
	1250 1250 1450 1250
Wire Wire Line
	1750 1250 2200 1250
Wire Wire Line
	2200 1350 1750 1350
Wire Wire Line
	1750 1450 2200 1450
Wire Wire Line
	2200 1550 1750 1550
Wire Wire Line
	1750 1650 2200 1650
Wire Wire Line
	2200 1750 1750 1750
Wire Wire Line
	1750 1850 2200 1850
Wire Wire Line
	2200 1950 1750 1950
Wire Wire Line
	1450 1350 1250 1350
Wire Wire Line
	1250 1450 1450 1450
Wire Wire Line
	1450 1550 1250 1550
Wire Wire Line
	1250 1650 1450 1650
Wire Wire Line
	1450 1750 1250 1750
Wire Wire Line
	1250 1850 1450 1850
Wire Wire Line
	1450 1950 1250 1950
$Comp
L power:GND #PWR02
U 1 1 626D8AA1
P 2900 2150
F 0 "#PWR02" H 2900 1900 50  0001 C CNN
F 1 "GND" H 2905 1977 50  0000 C CNN
F 2 "" H 2900 2150 50  0001 C CNN
F 3 "" H 2900 2150 50  0001 C CNN
	1    2900 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 1850 2900 1950
Wire Wire Line
	2800 1850 2900 1850
Wire Wire Line
	2800 1950 2900 1950
Connection ~ 2900 1950
Wire Wire Line
	2900 1950 2900 2150
$Comp
L Connector:Conn_01x05_Male J1
U 1 1 626EB930
P 1100 2850
F 0 "J1" H 1208 3231 50  0000 C CNN
F 1 "Conn_01x05_Male" H 1208 3140 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 1100 2850 50  0001 C CNN
F 3 "~" H 1100 2850 50  0001 C CNN
	1    1100 2850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Male J2
U 1 1 626ED811
P 1100 3800
F 0 "J2" H 1208 4181 50  0000 C CNN
F 1 "Conn_01x05_Male" H 1208 4090 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 1100 3800 50  0001 C CNN
F 3 "~" H 1100 3800 50  0001 C CNN
	1    1100 3800
	1    0    0    -1  
$EndComp
Text GLabel 1500 2750 2    50   Input ~ 0
A
Text GLabel 1500 2650 2    50   Input ~ 0
B
Text GLabel 1500 2850 2    50   Input ~ 0
F
Text GLabel 1500 2950 2    50   Input ~ 0
G
Text GLabel 1500 3600 2    50   Input ~ 0
DP
Text GLabel 1500 3700 2    50   Input ~ 0
C
Text GLabel 1500 3800 2    50   Input ~ 0
D
Text GLabel 1500 3900 2    50   Input ~ 0
E
$Comp
L power:GND #PWR0101
U 1 1 626F362E
P 1500 3150
F 0 "#PWR0101" H 1500 2900 50  0001 C CNN
F 1 "GND" H 1505 2977 50  0000 C CNN
F 2 "" H 1500 3150 50  0001 C CNN
F 3 "" H 1500 3150 50  0001 C CNN
	1    1500 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 626F4DB9
P 1500 4100
F 0 "#PWR0102" H 1500 3850 50  0001 C CNN
F 1 "GND" H 1505 3927 50  0000 C CNN
F 2 "" H 1500 4100 50  0001 C CNN
F 3 "" H 1500 4100 50  0001 C CNN
	1    1500 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 2650 1500 2650
Wire Wire Line
	1500 2750 1300 2750
Wire Wire Line
	1300 2850 1500 2850
Wire Wire Line
	1500 2950 1300 2950
Wire Wire Line
	1300 3050 1500 3050
Wire Wire Line
	1500 3050 1500 3150
Wire Wire Line
	1300 3600 1500 3600
Wire Wire Line
	1500 3700 1300 3700
Wire Wire Line
	1300 3800 1500 3800
Wire Wire Line
	1500 3900 1300 3900
Wire Wire Line
	1300 4000 1500 4000
Wire Wire Line
	1500 4000 1500 4100
Text GLabel 1250 1250 0    50   Input ~ 0
A
Text GLabel 1250 1350 0    50   Input ~ 0
B
Text GLabel 1250 1450 0    50   Input ~ 0
C
Text GLabel 1250 1550 0    50   Input ~ 0
D
Text GLabel 1250 1650 0    50   Input ~ 0
E
Text GLabel 1250 1750 0    50   Input ~ 0
F
Text GLabel 1250 1850 0    50   Input ~ 0
G
Text GLabel 1250 1950 0    50   Input ~ 0
DP
$EndSCHEMATC