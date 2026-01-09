# Game Design Document  
## Project Ironclad (Working Title)

---

# 1. High Concept

### Project summary  
Project Ironclad is a third-person action combat prototype built in Unreal Engine 5 using C++. The project focuses on responsive weapon-based combat, enemy AI, and modular gameplay systems, inspired by the camera language and combat readability of modern God of War.

### Portfolio goal  
Demonstrate professional-level gameplay programming, combat system architecture, AI integration, and Unreal Engine C++ workflows.

### Target role alignment  
Gameplay Programmer (Unreal, C++)

---

# 2. Experience Pillars

These guide every design and technical decision.

- **Combat feel first** – responsive, readable, animation-driven  
- **Systems-driven** – modular, extensible gameplay code  
- **Enemy interaction** – combat as a conversation, not target dummies  
- **Technical clarity** – features that clearly demonstrate engineering skill  

---

# 3. Player Fantasy

The player controls a heavily armed warrior in a contained combat arena, mastering positioning, timing, stamina, and weapon abilities to defeat increasingly aggressive enemies.

Focus: weight, impact, control, and tactical decision-making.

---

# 4. Core Gameplay Loop

1. Explore combat arena  
2. Engage enemies  
3. Read behavior and patterns  
4. Attack, dodge, manage stamina  
5. Defeat enemies  
6. Recover, upgrade, re-engage  

Supporting loops:
- Ability cooldown management  
- Risk vs reward combat decisions  
- Enemy prioritization  

---

# 5. Controls & Camera

### Camera style  
- Over-the-shoulder third-person  
- Soft lock-on system  
- Dynamic combat framing  
- God of War–style distance and height  
- Contextual camera assist during combat

### Core controls (initial)

- Move  
- Camera  
- Light attack  
- Heavy attack  
- Dodge  
- Block / parry (optional)  
- Ability 1 / Ability 2  
- Lock-on  
- Interact  

---

# 6. Player Systems

## 6.1 Character Controller  
- Custom C++ character class  
- Tuned acceleration, deceleration, and rotation  
- Animation-driven locomotion  
- Root motion for combat actions

## 6.2 Attributes  
- Health  
- Stamina  
- (Optional later) Rage / energy / mana  

All attributes managed through a dedicated component.

## 6.3 Combat System  
- Weapon-based melee combat  
- Light/heavy attack chains  
- Dodge with stamina cost  
- Hit detection via traces  
- Damage pipeline  
- Hit reactions and knockback  
- Death handling

## 6.4 Ability System  
- Abilities as modular gameplay objects  
- Cooldowns  
- Resource costs  
- Data-driven tuning (DataAssets)

Examples:
- Charged strike  
- Gap closer  
- Area knockback  
- Weapon throw

---

# 7. Enemy Design

## 7.1 Enemy Roles (initial)

**Enemy Type A – Aggressor**  
- Fast  
- Close-range  
- Pressure-focused  

**Enemy Type B – Controller**  
- Slower  
- Mid-range  
- Area denial or heavy attacks  

## 7.2 Enemy Capabilities  
- Perception system  
- Combat decision-making  
- Attack patterns  
- Cooldowns  
- Reactions to player state  

## 7.3 AI Goals  
- Spacing management  
- Player pressure  
- Recovery windows  
- Group behavior support (later)

---

# 8. AI Architecture (Design View)

- Base enemy C++ class  
- Modular components (health, combat, perception)  
- Behavior Tree or custom FSM  
- Blackboard-driven decisions  
- Attack selection system  
- Debug visualizers  

(This section will later reference architecture.md.)

---

# 9. Game Features

- HUD (health, stamina, cooldowns)  
- Lock-on targeting  
- Save/load  
- Pause & settings menu  
- Combat arena progression  
- Debug overlays  

---

# 10. Technical Design Goals

This project must demonstrate:

- Clean Unreal C++ architecture  
- Separation of gameplay systems  
- Reusable components  
- Animation-driven gameplay  
- AI-system integration  
- Performance awareness  
- Professional documentation

---

# 11. Scope Boundaries

Explicit non-goals:

- No open world  
- No narrative focus  
- No multiplayer  
- No asset production emphasis  

All effort goes into:
Gameplay, systems, AI, and polish.

---

# 12. Milestone Reference

This project follows the milestone plan defined in `milestones.md`.

Phases:
- Foundation  
- Combat  
- AI  
- Game features  
- Presentation  

---

# 13. Success Criteria

This project is successful if:

- Combat feels responsive and readable  
- At least two distinct enemy archetypes exist  
- Systems are modular and documented  
- Recruiters can understand your contribution in under 2 minutes  
- The project supports technical discussion in interviews  

---

# 14. Visual & Presentation Targets

- 60–90 second gameplay trailer  
- Short combat GIFs  
- System breakdown diagrams  
- Playable Windows build  
- Technical devlogs  

---

# 15. Living Document

This GDD evolves throughout development. All major design changes must be reflected here.
