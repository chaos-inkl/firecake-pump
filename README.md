
Pin assignments
---------------

| Arduino Pin    | Proper Name | Function         |
|----------------|-------------|------------------|
| digital pin 0  | PD0         | RXD              |
| digital pin 1  | PD1         | TXD              |
| digital pin 2  | PD2         | External Step    |
| digital pin 3  | PD3         | External Dir     |
| digital pin 4  | PD4         | Step Pump 1      |
| digital pin 5  | PD5         | Dir Pump 1       |
| digital pin 6  | PD6         | Limit min Pump 1 |
| digital pin 7  | PD7         | Limit max Pump 1 |
| digital pin 8  | PB0         | Motors disable   |
| digital pin 9  | PB1         |                  |
| digital pin 10 | PB2         | Step Pump 2      |
| digital pin 11 | PB3         | Dir Pump 2       |
| digital pin 12 | PB4         | Limit min Pump 2 |   
| digital pin 13 | PB5         | Limit max Pump 2 |
| analog input 0 | PC0         | Servo Pump1      |
| analog input 1 | PC1         | Servo Pump2      |
| analog input 2 | PC2         |                  |
| analog input 3 | PC3         | Idle signal      |
| analog input 4 | PC4         | Setup Button     |
| analog input 5 | PC5         | Ready signal     |

Idle and Setup are active low


Notes for Marlin
----------------
RAMPS 1.4 see http://reprap.org/mediawiki/images/archive/c/ca/20131210184132!Arduinomega1-4connectors.png

* D16 is idle
* D17 is setup
* D23 is ready

Procedures
-----------

*Setup:*
```
M42 P17 S0
M42 P17 S255
```

*G-Coder header:*
```
M42 P16 S255
M42 P17 S255

M226 P23 S-1
```

*G-Code footer:*
```
M42 P16 S0
```
