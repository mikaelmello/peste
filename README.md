# Peste

Peste, "Plague" in Portuguese, is an interactive thriller game where the main character, Hope, is a nurse hired to take care of Terry, a sick child abandoned by his parents, after they've lost all hope of finding a cure for him.

Terry's parents, Mary and Jekyll Hyde, are doctors with a great life until their only child, Terry, contracts the Plague, an incurable disease neglected by the medical academy because it only affected rats, a minority that was even rarer in the city's elite.

Their only hope was to find a cure themselves, and even though their expertise was far from this area, they still tried. And failed. Nothing worked until they found out about ancient medical books and their dark arts, having started to experiment with real children, from poorer families, that have also contracted the Plague.

After one of those kids, Neil, started showing signs of improvement after the last experiment, Jekyll got anxious and decided to apply the medicine in Terry, a mistake that he would later regret. Neil would later become largely violent and was instantly shot dead by Jekyll.

Afraid of what could happen to their kid, Mary and Jekyll decided to lock him in the house and run away, calling doctors, nurses and workers of the medical area in general to help Terry, without ever telling them what had really happened.

Many of them got killed or ran away, leaving Terry's parents with nothing but despair.

Except for their last chance.

Hope.

## The Game

Your goal is to leave the house via the entrance door, with or without Terry.

### Controls

#### Keyboard

- **Arrows**: Walk
- **Shift**: Run while walking
- **X**: Interactions
- **Space bar**: Select items, skip dialogues
- **Escape key**: Pause the game (or open inventory), go back to menu.

#### Joystick (Xbox/PS)

- **Left analog**: Walk
- **RB or R1**: Run while walking
- **A or X**: Interactions, select items
- **B or Circle**: Skip dialogues, go back to menu.
- **Start**: Pause the game (or open inventory).

## Team Members

All team members contributed to all areas, always reviewing code and refactoring whenever we discussed a better alternative, however some features can be attributed more to one member than another:

### Gabriel Levi

Responsible for building the map and adding interactions with items or furniture, rendering relative to positions, door and stairs mechanics.

### Gabriel Nunes

Main developer of the Antagonist and its finite state machine along with a path-finding algorithm, the main menu, the ending states.

### Mikael Mello

Built the dynamic grid-based collision system, adding (and sometimes creating) sound effects, dialogues, inventory system, joystick implementation, also important improvements over other key sections of the code.

## How to Play

You are **strongly** recommended to play using Linux.

- Install SDL: `sudo apt‐get install build-essentials libsdl2‐dev libsdl2‐image‐dev libsdl2‐mixer‐dev libsdl2-ttf-dev libsd`
- Compile: `make -j`
- Run: `./peste`
