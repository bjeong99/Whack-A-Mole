# Whack-A-Mole
 For our project, we made a Whack-A-Mole game including a
  - K64F microcontroller
  - 32x32 Adafruit LED Matrix Panel
  - and homemade controller. 
  
The goal of the game is to have a player press a series of nine buttons that correspond to the nine holes displayed on the screen. A mole will appear out of one of these holes, and the player will have to press the button corresponding to that hole to score a point. As the game continues, the time window that the user is able to press a button will get shorter and shorter. To make the game more fun, we included two levels of difficulty. In the startup screen, the user will be able to choose an “easy” mode or a “hard” mode. In the easy mode, the moles appear slower and the game only ends if the user does not press the correct button in time (incorrect inputs are ignored). In the hard mode, the game ends if the user does not press the correct button in time and any incorrect inputs will automatically end the game.
