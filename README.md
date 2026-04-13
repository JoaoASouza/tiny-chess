# Tiny Chess

Tiny Chess is a simple terminal-based chess engine and interface written in C. It supports local multiplayer and playing against the Stockfish chess engine as an AI opponent. The project demonstrates basic chess logic, move validation, FEN serialization, and simple game state management.

## Features
- Play chess in the terminal
- Local multiplayer mode
- Play against Stockfish AI (requires Stockfish installed and available in PATH)
- Save and load games

## Compilation

To compile the project, make sure you have `gcc` installed. Run the following command in the project directory:

```
gcc -o tiny-chess main.c engine.c renderer.c
```

This will produce an executable named `tiny-chess`.

## Playing the Game

Run the game with:

```
./tiny-chess
```

You will be presented with a menu:
- Select `1` for Local Multiplayer
- Select `2` to play against the AI (Stockfish)
- Select `0` to exit

### Controls
- Enter moves in standard algebraic notation (e.g., `e2e4`)
- Enter `q` to quit the game
- You will be prompted to save or load games at the start/end

### Requirements
- [Stockfish](https://stockfishchess.org/) must be installed and available in your system's PATH to play against the AI.

## License

This project is licensed under the MIT License.
