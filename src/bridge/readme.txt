There are two implementations:
1) using Serial (to abstract SPI and I2C)
2) using I2C directly

Originally used Serial so I could switch back and forth from SPI to I2C on different boards.
When I encountered problems, I simplified to using I2C directly.