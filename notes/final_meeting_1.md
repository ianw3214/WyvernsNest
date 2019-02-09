# FINAL MEETING 1

## Customization state

- Flash skill tree button when skill points available
  - Skill points determined by `player level - selected nodes`
- ~~How to continue from customization state back into game state?~~
- ~~Player attacks loadout~~
  - ~~Render chosen player attacks~~
  - ~~Need to store skill tree node id -> attacks **map**~~
  - ~~Ability to change player attack loadout~~

## Skill Tree state

- Display descriptions
- Skill tree **link** rendering -> *NOT IMPORTANT*
  - Use rects to render instead of lines
  - Write **shaders** to make them render nicer
- Implement skill tree **panning**
- **Highlight** the current node that the cursor is hovering on

## Menu state

- Some nice **shaders** to make the menu look nice
- ~~**Settings** menu~~
  - ~~Change window size~~
  - ~~Change music / sound effect volume~~
- ~~**Credits** menu~~
  - ~~Setup basic structure to easily fill out names -> role~~
- ~~**Back** to main menu button for both~~

## Combat state

- ~~Order unit turns based on **DEX** attribute~~
- More animations
  - If programmer, just fill with squares or some shit for now
  - 1 frame animations for speed
- Particle effects
  - ~~Effect for attacks~~
  - Effect for passive status
- ~~Delay between killing enemies and showing menu~~
- ~~What happens when the player dies?~~
- ~~Make *ZIGUIGUI LINE OF PATH* look nice~~
- ~~Finalize **controls**~~
  - ~~Use arrow keys + enter/space like in menu state~~
  - ~~Add mouse control support~~

## Other

- Level/Encounter design
- Game balance
- ~~Change escape key to **NOT** exit the game~~
- Clean up a bunch of **TODOs** in the code
- ~~Implement EXP -> Store EXP gained in the level code~~
