EESchema Schematic File Version 4
LIBS:sheild_exp430g2-cache
LIBS:veiculo_uno-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "lun. 30 mars 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 8550 600  0    60   ~ 0
Shield for EXP430G2\n
Wire Notes Line
	8525 825  9925 825 
Wire Notes Line
	9925 825  9925 475 
$Comp
L Connector_Generic:Conn_01x10 J2
U 1 1 5EE2261F
P 10050 1500
F 0 "J2" H 10050 2050 50  0000 C CNN
F 1 "Digital" V 10150 1500 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x10" V 10200 1500 20  0000 C CNN
F 3 "" H 10050 1500 50  0000 C CNN
	1    10050 1500
	-1   0    0    -1  
$EndComp
Wire Notes Line
	8500 3450 11200 3450
Text GLabel 10400 1200 2    50   Input ~ 0
XIN
Text GLabel 10400 1300 2    50   Input ~ 0
XOUT
Text GLabel 10400 1400 2    50   Input ~ 0
TEST
Text GLabel 10400 1500 2    50   Input ~ 0
RST
Text GLabel 10400 1600 2    50   Input ~ 0
P1.7
Text GLabel 10400 1700 2    50   Input ~ 0
P1.6
Text GLabel 10400 1800 2    50   Input ~ 0
P2.5
Text GLabel 10400 1900 2    50   Input ~ 0
P2.4
Text GLabel 10400 2000 2    50   Input ~ 0
P2.3
Text GLabel 9150 1300 0    50   Input ~ 0
P1.1
Text GLabel 9150 1400 0    50   Input ~ 0
P1.2
Text GLabel 9150 1500 0    50   Input ~ 0
P1.3
Text GLabel 9150 1600 0    50   Input ~ 0
P1.4
Text GLabel 9150 1700 0    50   Input ~ 0
P1.5
Text GLabel 9150 1800 0    50   Input ~ 0
P2.0
Text GLabel 9150 1900 0    50   Input ~ 0
P2.1
Text GLabel 9150 2000 0    50   Input ~ 0
P2.2
$Comp
L power:+3.3V #PWR01
U 1 1 5EE1925F
P 9050 1050
F 0 "#PWR01" H 9050 900 50  0001 C CNN
F 1 "+3.3V" V 9150 1150 50  0000 C CNN
F 2 "" H 9050 1050 50  0000 C CNN
F 3 "" H 9050 1050 50  0000 C CNN
	1    9050 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 1050 9050 1100
Wire Wire Line
	9050 1100 9300 1100
$Comp
L power:GND #PWR06
U 1 1 5EE19BA5
P 10300 2100
F 0 "#PWR06" H 10300 1850 50  0001 C CNN
F 1 "GND" H 10300 1950 50  0000 C CNN
F 2 "" H 10300 2100 50  0000 C CNN
F 3 "" H 10300 2100 50  0000 C CNN
	1    10300 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 2100 10300 1100
Wire Wire Line
	10250 1100 10300 1100
Wire Wire Line
	9350 1700 9400 1700
Text Notes 9700 1600 0    60   ~ 0
1
$Comp
L Connector:Conn_01x02_Male J8
U 1 1 5E6BC9D5
P 5300 1650
F 0 "J8" H 5406 1828 50  0000 C CNN
F 1 "Conn_01x02_Male" V 5800 1600 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5300 1650 50  0001 C CNN
F 3 "~" H 5300 1650 50  0001 C CNN
	1    5300 1650
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J12
U 1 1 5E6C3DB8
P 5200 3650
F 0 "J12" V 5353 3463 50  0000 R CNN
F 1 "Conn_01x02_Male" V 5262 3463 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5200 3650 50  0001 C CNN
F 3 "~" H 5200 3650 50  0001 C CNN
	1    5200 3650
	0    -1   -1   0   
$EndComp
Text GLabel 5150 1300 0    50   Input ~ 0
ADC3
Text GLabel 5500 1300 2    50   Input ~ 0
A3
Wire Wire Line
	5500 1300 5400 1300
Wire Wire Line
	5400 1300 5400 1450
Wire Wire Line
	5150 1300 5300 1300
Wire Wire Line
	5300 1300 5300 1450
Text GLabel 5400 3300 2    50   Input ~ 0
2
Wire Wire Line
	5400 3300 5300 3300
Wire Wire Line
	5300 3300 5300 3450
Text GLabel 5100 3300 0    50   Input ~ 0
Echo1
Wire Wire Line
	5200 3450 5200 3300
Wire Wire Line
	5200 3300 5100 3300
$Comp
L Connector:Conn_01x02_Male J20
U 1 1 5E6D99F8
P 2700 6200
F 0 "J20" V 2853 6013 50  0000 R CNN
F 1 "Conn_01x02_Male" V 2762 6013 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2700 6200 50  0001 C CNN
F 3 "~" H 2700 6200 50  0001 C CNN
	1    2700 6200
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J19
U 1 1 5E6DCAD9
P 1450 6200
F 0 "J19" V 1603 6013 50  0000 R CNN
F 1 "Conn_01x02_Male" V 1512 6013 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1450 6200 50  0001 C CNN
F 3 "~" H 1450 6200 50  0001 C CNN
	1    1450 6200
	0    -1   -1   0   
$EndComp
Text GLabel 2900 5850 2    50   Input ~ 0
A5(SCL)
Text GLabel 1650 5850 2    50   Input ~ 0
A4(SDA)
Wire Wire Line
	2900 5850 2800 5850
Wire Wire Line
	2800 5850 2800 6000
Wire Wire Line
	1550 5850 1550 6000
Wire Wire Line
	1550 5850 1650 5850
Text GLabel 1350 5850 0    50   Input ~ 0
R_TCRT
Wire Wire Line
	1350 5850 1450 5850
Wire Wire Line
	1450 5850 1450 6000
Text GLabel 2650 5850 0    50   Input ~ 0
L_TCRT
Wire Wire Line
	2650 5850 2700 5850
Wire Wire Line
	2700 5850 2700 6000
$Comp
L Connector:Conn_01x02_Male J21
U 1 1 5E6E73BA
P 4700 6900
F 0 "J21" V 4853 6713 50  0000 R CNN
F 1 "Conn_01x02_Male" V 4762 6713 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4700 6900 50  0001 C CNN
F 3 "~" H 4700 6900 50  0001 C CNN
	1    4700 6900
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J13
U 1 1 5E6E742E
P 6500 3650
F 0 "J13" V 6653 3463 50  0000 R CNN
F 1 "Conn_01x02_Male" V 6562 3463 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6500 3650 50  0001 C CNN
F 3 "~" H 6500 3650 50  0001 C CNN
	1    6500 3650
	0    -1   -1   0   
$EndComp
Text GLabel 4850 6550 2    50   Input ~ 0
13
Wire Wire Line
	4850 6550 4800 6550
Wire Wire Line
	4800 6550 4800 6700
Text GLabel 4650 6550 0    50   Input ~ 0
D0
Wire Wire Line
	4650 6550 4700 6550
Wire Wire Line
	4700 6550 4700 6700
Text GLabel 6650 3300 2    50   Input ~ 0
4
Text GLabel 6450 3300 0    50   Input ~ 0
Trig1
Wire Wire Line
	6650 3300 6600 3300
Wire Wire Line
	6600 3300 6600 3450
Wire Wire Line
	6450 3300 6500 3300
Wire Wire Line
	6500 3300 6500 3450
$Comp
L Connector:Conn_01x02_Male J11
U 1 1 5E6FE4FA
P 6500 3150
F 0 "J11" V 6653 2963 50  0000 R CNN
F 1 "Conn_01x02_Male" V 6562 2963 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6500 3150 50  0001 C CNN
F 3 "~" H 6500 3150 50  0001 C CNN
	1    6500 3150
	0    -1   -1   0   
$EndComp
Text GLabel 6650 2800 2    50   Input ~ 0
7
Text GLabel 6450 2800 0    50   Input ~ 0
Trig0
Wire Wire Line
	6450 2800 6500 2800
Wire Wire Line
	6500 2800 6500 2950
Wire Wire Line
	6600 2950 6600 2800
Wire Wire Line
	6600 2800 6650 2800
$Comp
L Connector:Conn_01x02_Male J10
U 1 1 5E70305F
P 5200 3150
F 0 "J10" V 5353 2963 50  0000 R CNN
F 1 "Conn_01x02_Male" V 5262 2963 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5200 3150 50  0001 C CNN
F 3 "~" H 5200 3150 50  0001 C CNN
	1    5200 3150
	0    -1   -1   0   
$EndComp
Text GLabel 5150 2800 0    50   Input ~ 0
Echo0
Wire Wire Line
	5150 2800 5200 2800
Wire Wire Line
	5200 2800 5200 2950
Text GLabel 5350 2800 2    50   Input ~ 0
8
Wire Wire Line
	5300 2950 5300 2800
Wire Wire Line
	5300 2800 5350 2800
Text GLabel 4850 7000 2    50   Input ~ 0
12
Text GLabel 10050 4200 2    50   Input ~ 0
3
$Comp
L Connector:Conn_01x02_Male J24
U 1 1 5E7089E9
P 4700 7350
F 0 "J24" V 4853 7163 50  0000 R CNN
F 1 "Conn_01x02_Male" V 4762 7163 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4700 7350 50  0001 C CNN
F 3 "~" H 4700 7350 50  0001 C CNN
	1    4700 7350
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J18
U 1 1 5E708A39
P 9900 4550
F 0 "J18" V 10053 4363 50  0000 R CNN
F 1 "Conn_01x02_Male" V 9962 4363 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9900 4550 50  0001 C CNN
F 3 "~" H 9900 4550 50  0001 C CNN
	1    9900 4550
	0    -1   -1   0   
$EndComp
Text GLabel 4650 7000 0    50   Input ~ 0
D1
Wire Wire Line
	4650 7000 4700 7000
Wire Wire Line
	4700 7000 4700 7150
Wire Wire Line
	4850 7000 4800 7000
Wire Wire Line
	4800 7000 4800 7150
Wire Wire Line
	10050 4200 10000 4200
Wire Wire Line
	10000 4200 10000 4350
Text GLabel 9850 4200 0    50   Input ~ 0
PWM2N
Wire Wire Line
	9850 4200 9900 4200
Wire Wire Line
	9900 4200 9900 4350
Text GLabel 5100 650  0    50   Input ~ 0
ADC0
$Comp
L Connector:Conn_01x02_Male J6
U 1 1 5E718E4F
P 5150 1000
F 0 "J6" V 5303 813 50  0000 R CNN
F 1 "Conn_01x02_Male" V 5212 813 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5150 1000 50  0001 C CNN
F 3 "~" H 5150 1000 50  0001 C CNN
	1    5150 1000
	0    -1   -1   0   
$EndComp
Text GLabel 5300 650  2    50   Input ~ 0
A2
Wire Wire Line
	5300 650  5250 650 
Wire Wire Line
	5250 650  5250 800 
Wire Wire Line
	5100 650  5150 650 
Wire Wire Line
	5150 650  5150 800 
Text GLabel 6050 650  0    50   Input ~ 0
ADC1
$Comp
L Connector:Conn_01x02_Male J7
U 1 1 5E722316
P 6100 1000
F 0 "J7" V 6253 813 50  0000 R CNN
F 1 "Conn_01x02_Male" V 6162 813 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6100 1000 50  0001 C CNN
F 3 "~" H 6100 1000 50  0001 C CNN
	1    6100 1000
	0    -1   -1   0   
$EndComp
Text GLabel 6250 650  2    50   Input ~ 0
A1
Wire Wire Line
	6250 650  6200 650 
Wire Wire Line
	6200 650  6200 800 
Wire Wire Line
	6050 650  6100 650 
Wire Wire Line
	6100 650  6100 800 
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 5E7298F1
P 7050 1000
F 0 "J3" V 7203 813 50  0000 R CNN
F 1 "Conn_01x02_Male" V 7112 813 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7050 1000 50  0001 C CNN
F 3 "~" H 7050 1000 50  0001 C CNN
	1    7050 1000
	0    -1   -1   0   
$EndComp
Text GLabel 7000 700  0    50   Input ~ 0
ADC2
Text GLabel 7200 700  2    50   Input ~ 0
A0
Wire Wire Line
	7000 700  7050 700 
Wire Wire Line
	7050 700  7050 800 
Wire Wire Line
	7200 700  7150 700 
Wire Wire Line
	7150 700  7150 800 
Text GLabel 6150 1300 0    50   Input ~ 0
AMP1
$Comp
L Connector:Conn_01x02_Male J4
U 1 1 5E735276
P 6550 1400
F 0 "J4" V 6703 1213 50  0000 R CNN
F 1 "Conn_01x02_Male" H 7150 1700 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6550 1400 50  0001 C CNN
F 3 "~" H 6550 1400 50  0001 C CNN
	1    6550 1400
	-1   0    0    1   
$EndComp
Wire Wire Line
	6250 1450 6250 1400
Wire Wire Line
	6250 1400 6350 1400
$Comp
L Connector:Conn_01x02_Male J23
U 1 1 5E74836B
P 2450 7250
F 0 "J23" V 2603 7063 50  0000 R CNN
F 1 "Conn_01x02_Male" V 2512 7063 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2450 7250 50  0001 C CNN
F 3 "~" H 2450 7250 50  0001 C CNN
	1    2450 7250
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J22
U 1 1 5E7483D7
P 1500 7250
F 0 "J22" V 1653 7063 50  0000 R CNN
F 1 "Conn_01x02_Male" V 1562 7063 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1500 7250 50  0001 C CNN
F 3 "~" H 1500 7250 50  0001 C CNN
	1    1500 7250
	0    -1   -1   0   
$EndComp
Text GLabel 2600 6950 2    50   Input ~ 0
Rx
Text GLabel 1650 6950 2    50   Input ~ 0
Tx
Text GLabel 1450 6950 0    50   Input ~ 0
RXB
Text GLabel 2400 6950 0    50   Input ~ 0
TXB
Wire Wire Line
	2600 6950 2550 6950
Wire Wire Line
	2550 6950 2550 7050
Wire Wire Line
	2400 6950 2450 6950
Wire Wire Line
	2450 6950 2450 7050
Wire Wire Line
	1650 6950 1600 6950
Wire Wire Line
	1600 6950 1600 7050
Wire Wire Line
	1450 6950 1500 6950
Wire Wire Line
	1500 6950 1500 7050
$Comp
L Connector:Conn_01x02_Male J15
U 1 1 5E76127F
P 8900 4050
F 0 "J15" V 9053 3863 50  0000 R CNN
F 1 "Conn_01x02_Male" V 8962 3863 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8900 4050 50  0001 C CNN
F 3 "~" H 8900 4050 50  0001 C CNN
	1    8900 4050
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J16
U 1 1 5E7612D7
P 9900 4050
F 0 "J16" V 10053 3863 50  0000 R CNN
F 1 "Conn_01x02_Male" V 9962 3863 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9900 4050 50  0001 C CNN
F 3 "~" H 9900 4050 50  0001 C CNN
	1    9900 4050
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J17
U 1 1 5E761337
P 8900 4550
F 0 "J17" V 9053 4363 50  0000 R CNN
F 1 "Conn_01x02_Male" V 8962 4363 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8900 4550 50  0001 C CNN
F 3 "~" H 8900 4550 50  0001 C CNN
	1    8900 4550
	0    -1   -1   0   
$EndComp
Text GLabel 8850 3750 0    50   Input ~ 0
PWM1
Text GLabel 9850 3750 0    50   Input ~ 0
PWM1N
Text GLabel 8850 4250 0    50   Input ~ 0
PWM2
Wire Wire Line
	8850 3750 8900 3750
Wire Wire Line
	8900 3750 8900 3850
Wire Wire Line
	9850 3750 9900 3750
Wire Wire Line
	9900 3750 9900 3850
Wire Wire Line
	8850 4250 8900 4250
Wire Wire Line
	8900 4250 8900 4350
Text GLabel 9050 3750 2    50   Input ~ 0
6
Wire Wire Line
	9050 3750 9000 3750
Wire Wire Line
	9000 3750 9000 3850
Text GLabel 10050 3750 2    50   Input ~ 0
5
Text GLabel 9050 4250 2    50   Input ~ 0
11
Wire Wire Line
	9050 4250 9000 4250
Wire Wire Line
	9000 4250 9000 4350
Wire Wire Line
	10050 3750 10000 3750
Wire Wire Line
	10000 3750 10000 3850
$Comp
L Connector_Generic:Conn_02x13_Odd_Even J9
U 1 1 5EE18C86
P 1800 2350
F 0 "J9" H 1850 3167 50  0000 C CNN
F 1 "Conn_02x13_Odd_Even" H 1850 3076 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x13_P2.54mm_Vertical" H 1800 2350 50  0001 C CNN
F 3 "~" H 1800 2350 50  0001 C CNN
	1    1800 2350
	1    0    0    -1  
$EndComp
Text GLabel 2550 1750 2    50   Input ~ 0
D0
Wire Wire Line
	2550 1750 2100 1750
$Comp
L power:GND #PWR08
U 1 1 5EE1DC02
P 2250 3050
F 0 "#PWR08" H 2250 2800 50  0001 C CNN
F 1 "GND" H 2250 2900 50  0000 C CNN
F 2 "" H 2250 3050 50  0000 C CNN
F 3 "" H 2250 3050 50  0000 C CNN
	1    2250 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 1850 2250 1850
Wire Wire Line
	2250 1850 2250 2550
$Comp
L power:+5V #PWR04
U 1 1 5EE2252F
P 1400 1650
F 0 "#PWR04" H 1400 1500 50  0001 C CNN
F 1 "+5V" V 1400 1850 50  0000 C CNN
F 2 "" H 1400 1650 50  0000 C CNN
F 3 "" H 1400 1650 50  0000 C CNN
	1    1400 1650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR05
U 1 1 5EE2270A
P 2400 1650
F 0 "#PWR05" H 2400 1500 50  0001 C CNN
F 1 "+5V" V 2400 1850 50  0000 C CNN
F 2 "" H 2400 1650 50  0000 C CNN
F 3 "" H 2400 1650 50  0000 C CNN
	1    2400 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 2450 2400 2450
Wire Wire Line
	2400 2450 2400 1650
Wire Wire Line
	1400 2450 1400 1650
$Comp
L power:GND #PWR07
U 1 1 5EE2BB45
P 1500 3050
F 0 "#PWR07" H 1500 2800 50  0001 C CNN
F 1 "GND" H 1500 2900 50  0000 C CNN
F 2 "" H 1500 3050 50  0000 C CNN
F 3 "" H 1500 3050 50  0000 C CNN
	1    1500 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 3050 1500 1750
Wire Wire Line
	1500 1750 1600 1750
Wire Wire Line
	1600 2450 1400 2450
Text GLabel 2550 1950 2    50   Input ~ 0
Echo1
Text GLabel 2550 2050 2    50   Input ~ 0
Trig1
Text GLabel 2550 2250 2    50   Input ~ 0
R_TCRT
Text GLabel 2550 2350 2    50   Input ~ 0
L_TCRT
Text GLabel 2550 2650 2    50   Input ~ 0
Trig0
Text GLabel 2550 2750 2    50   Input ~ 0
Echo0
Text GLabel 2550 2850 2    50   Input ~ 0
D1
Text GLabel 2550 2950 2    50   Input ~ 0
PWM2N
Text GLabel 1300 2950 0    50   Input ~ 0
PWM2
Text GLabel 1300 2850 0    50   Input ~ 0
PWM1N
Text GLabel 1300 2750 0    50   Input ~ 0
PWM1
Text GLabel 1300 2650 0    50   Input ~ 0
RXB
Text GLabel 1300 2550 0    50   Input ~ 0
TXB
Text GLabel 1300 2350 0    50   Input ~ 0
AMP2
Text GLabel 1300 2250 0    50   Input ~ 0
ADC2
Text GLabel 1300 1850 0    50   Input ~ 0
AMP1
Text GLabel 1300 2150 0    50   Input ~ 0
ADC1
Text GLabel 1300 2050 0    50   Input ~ 0
ADC0
Text GLabel 1300 1950 0    50   Input ~ 0
ADC3
Wire Wire Line
	1300 2950 1600 2950
Wire Wire Line
	1600 2850 1300 2850
Wire Wire Line
	1300 2750 1600 2750
Wire Wire Line
	1300 2650 1600 2650
Wire Wire Line
	1600 2550 1300 2550
Wire Wire Line
	1300 2350 1600 2350
Wire Wire Line
	1600 2250 1300 2250
Wire Wire Line
	1300 2150 1600 2150
Wire Wire Line
	1300 2050 1600 2050
Wire Wire Line
	1300 1950 1600 1950
Wire Wire Line
	1300 1850 1600 1850
Wire Wire Line
	2100 2950 2550 2950
Wire Wire Line
	2550 2850 2100 2850
Wire Wire Line
	2100 2750 2550 2750
Wire Wire Line
	2550 2650 2100 2650
Wire Wire Line
	2100 2350 2550 2350
Wire Wire Line
	2100 2250 2550 2250
Wire Wire Line
	2100 2050 2550 2050
Wire Wire Line
	2550 1950 2100 1950
$Comp
L power:GND #PWR02
U 1 1 5EEDDC4D
P 6250 1450
F 0 "#PWR02" H 6250 1200 50  0001 C CNN
F 1 "GND" H 6250 1300 50  0000 C CNN
F 2 "" H 6250 1450 50  0000 C CNN
F 3 "" H 6250 1450 50  0000 C CNN
	1    6250 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 1300 6350 1300
Text GLabel 7150 1300 0    50   Input ~ 0
AMP2
$Comp
L Connector:Conn_01x02_Male J5
U 1 1 5EEF3F3C
P 7550 1400
F 0 "J5" V 7703 1213 50  0000 R CNN
F 1 "Conn_01x02_Male" H 8150 1700 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7550 1400 50  0001 C CNN
F 3 "~" H 7550 1400 50  0001 C CNN
	1    7550 1400
	-1   0    0    1   
$EndComp
Wire Wire Line
	7250 1450 7250 1400
Wire Wire Line
	7250 1400 7350 1400
$Comp
L power:GND #PWR03
U 1 1 5EEF3F45
P 7250 1450
F 0 "#PWR03" H 7250 1200 50  0001 C CNN
F 1 "GND" H 7250 1300 50  0000 C CNN
F 2 "" H 7250 1450 50  0000 C CNN
F 3 "" H 7250 1450 50  0000 C CNN
	1    7250 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 1300 7350 1300
Text Notes 8750 5100 0    50   ~ 0
11 - OC2A\n3 - OC2B\n\n6 - OC0A\n5- OC0B
Text Notes 5150 4150 0    50   ~ 0
2 - int0 (ext. event)\n8 - ICP1 (capture)\n\n4 - GPIO\n7 - GPIO 
$Comp
L pspice:CAP C1
U 1 1 5EFE9E76
P 1500 4000
F 0 "C1" H 1678 4046 50  0000 L CNN
F 1 "100n" H 1678 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1500 4000 50  0001 C CNN
F 3 "~" H 1500 4000 50  0001 C CNN
	1    1500 4000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR010
U 1 1 5EFE9F72
P 1500 3700
F 0 "#PWR010" H 1500 3550 50  0001 C CNN
F 1 "+5V" V 1500 3900 50  0000 C CNN
F 2 "" H 1500 3700 50  0000 C CNN
F 3 "" H 1500 3700 50  0000 C CNN
	1    1500 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5EFEA0B3
P 1500 4350
F 0 "#PWR014" H 1500 4100 50  0001 C CNN
F 1 "GND" H 1500 4200 50  0000 C CNN
F 2 "" H 1500 4350 50  0000 C CNN
F 3 "" H 1500 4350 50  0000 C CNN
	1    1500 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 3700 1500 3750
Wire Wire Line
	1500 4250 1500 4350
$Comp
L pspice:CAP C2
U 1 1 5F00DDFD
P 2100 4000
F 0 "C2" H 2278 4046 50  0000 L CNN
F 1 "100n" H 2278 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2100 4000 50  0001 C CNN
F 3 "~" H 2100 4000 50  0001 C CNN
	1    2100 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5F00DE0A
P 2100 4350
F 0 "#PWR015" H 2100 4100 50  0001 C CNN
F 1 "GND" H 2100 4200 50  0000 C CNN
F 2 "" H 2100 4350 50  0000 C CNN
F 3 "" H 2100 4350 50  0000 C CNN
	1    2100 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 3700 2100 3750
Wire Wire Line
	2100 4250 2100 4350
$Comp
L power:+3.3V #PWR011
U 1 1 5F015339
P 2100 3700
F 0 "#PWR011" H 2100 3550 50  0001 C CNN
F 1 "+3.3V" V 2100 3950 50  0000 C CNN
F 2 "" H 2100 3700 50  0000 C CNN
F 3 "" H 2100 3700 50  0000 C CNN
	1    2100 3700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J14
U 1 1 5F015F8B
P 2800 4050
F 0 "J14" H 2906 4328 50  0000 C CNN
F 1 "Conn_01x04_Male" H 2906 4237 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2800 4050 50  0001 C CNN
F 3 "~" H 2800 4050 50  0001 C CNN
	1    2800 4050
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR013
U 1 1 5F0162BF
P 3300 3700
F 0 "#PWR013" H 3300 3550 50  0001 C CNN
F 1 "+5V" V 3300 3900 50  0000 C CNN
F 2 "" H 3300 3700 50  0000 C CNN
F 3 "" H 3300 3700 50  0000 C CNN
	1    3300 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR012
U 1 1 5F016456
P 3200 3700
F 0 "#PWR012" H 3200 3550 50  0001 C CNN
F 1 "+3.3V" V 3200 3950 50  0000 C CNN
F 2 "" H 3200 3700 50  0000 C CNN
F 3 "" H 3200 3700 50  0000 C CNN
	1    3200 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5F02C3E1
P 3200 4350
F 0 "#PWR016" H 3200 4100 50  0001 C CNN
F 1 "GND" H 3200 4200 50  0000 C CNN
F 2 "" H 3200 4350 50  0000 C CNN
F 3 "" H 3200 4350 50  0000 C CNN
	1    3200 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4250 3200 4350
Wire Wire Line
	3300 4150 3300 3950
Wire Wire Line
	3200 4050 3200 3700
Wire Wire Line
	3000 4250 3200 4250
Wire Wire Line
	3000 4150 3300 4150
Wire Wire Line
	3000 4050 3200 4050
Wire Wire Line
	3000 3950 3300 3950
Connection ~ 3300 3950
Wire Wire Line
	3300 3950 3300 3700
Text Notes 5650 6600 0    50   ~ 0
Encoder
Text Notes 1100 5700 0    50   ~ 0
Sensor optico
Wire Wire Line
	2100 2550 2250 2550
Connection ~ 2250 2550
Wire Wire Line
	2250 2550 2250 3050
$Comp
L Device:R R1
U 1 1 5EE3A0BC
P 3900 4250
F 0 "R1" H 3970 4296 50  0000 L CNN
F 1 "R" H 3970 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3830 4250 50  0001 C CNN
F 3 "~" H 3900 4250 50  0001 C CNN
	1    3900 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5EE3A180
P 3900 4500
F 0 "#PWR017" H 3900 4250 50  0001 C CNN
F 1 "GND" H 3900 4350 50  0000 C CNN
F 2 "" H 3900 4500 50  0000 C CNN
F 3 "" H 3900 4500 50  0000 C CNN
	1    3900 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5EE3A374
P 3900 3850
F 0 "D1" V 3938 3733 50  0000 R CNN
F 1 "LED" V 3847 3733 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 3900 3850 50  0001 C CNN
F 3 "~" H 3900 3850 50  0001 C CNN
	1    3900 3850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3900 3550 3900 3700
Wire Wire Line
	3900 4000 3900 4100
Wire Wire Line
	3900 4400 3900 4500
$Comp
L power:+5V #PWR09
U 1 1 5EE64533
P 3900 3550
F 0 "#PWR09" H 3900 3400 50  0001 C CNN
F 1 "+5V" V 3900 3750 50  0000 C CNN
F 2 "" H 3900 3550 50  0000 C CNN
F 3 "" H 3900 3550 50  0000 C CNN
	1    3900 3550
	1    0    0    -1  
$EndComp
Text Notes 7000 6750 0    50   ~ 0
SHIELD EXP430G2 PARA CARRINHO AUTÔNOMO\nLIGAÇÃO CONFORME PLACA DE POTÊNCIA
Wire Notes Line
	8500 500  8500 3450
$Comp
L Connector_Generic:Conn_01x10 J1
U 1 1 5EE1593E
P 9500 1500
F 0 "J1" H 9500 2050 50  0000 C CNN
F 1 "Digital" V 9600 1500 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x10" V 9650 1500 20  0000 C CNN
F 3 "" H 9500 1500 50  0000 C CNN
	1    9500 1500
	1    0    0    -1  
$EndComp
Text GLabel 9150 1200 0    50   Input ~ 0
P1.0
$EndSCHEMATC
