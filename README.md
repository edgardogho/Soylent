# Soylent
Metal Bender Electro Magnet control

This is a simple arduino project that basically reads an input, if the input goes to HIGH twice, it sets one output as HIGH. If the input goes back to HIGH it reset (output down) and goes back to counting. It displays how many times this happened on the LCD, and has a 9 second timeout when the output is HIGH (since it controls and electromagnet which would burn if its ON for a long time).
Its called soylent because the LCD is green and I had no other name for this.
