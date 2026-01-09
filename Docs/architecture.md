# Technical Architecture

## Project Ironclad -- Unreal Engine 5 (C++)

------------------------------------------------------------------------

# Purpose of This Document

This document defines the technical architecture of Project Ironclad. It
describes how gameplay systems are structured, how they interact, and
what responsibilities each layer owns.

This is a living technical blueprint. All major systems must align with
the principles defined here.

------------------------------------------------------------------------

# Architecture Goals

-   Gameplay systems are modular and reusable\
-   Responsibilities are clearly separated\
-   Systems communicate through clean interfaces\
-   Gameplay is animation-driven\
-   Data is designer-tunable\
-   AI, combat, and abilities are decoupled\
-   Code supports scalability and debugging

------------------------------------------------------------------------

# High-Level Architecture Overview

The project follows a layered gameplay architecture:

``` text
Player Input
   ↓
Character / Pawn Layer
   ↓
Gameplay Systems (Combat, Abilities, Attributes)
   ↓
World Interaction (AI, Weapons, Environment)
   ↓
Animation, UI, Audio, VFX
```

Core philosophy:\
Characters own **state**, systems own **behavior**.

------------------------------------------------------------------------

# Project Module Structure

``` text
Source/
├── Ironclad/
│   ├── Ironclad.cpp
│   ├── IroncladGameModeBase.h/.cpp
│   │
│   ├── Characters/
│   │   ├── IroncladCharacterBase.h/.cpp
│   │   ├── PlayerCharacter.h/.cpp
│   │   └── EnemyCharacterBase.h/.cpp
│   │
│   ├── Components/
│   │   ├── AttributeComponent.h/.cpp
│   │   ├── CombatComponent.h/.cpp
│   │   ├── AbilityComponent.h/.cpp
│   │   ├── TargetingComponent.h/.cpp
│   │   └── HealthComponent.h/.cpp
│   │
│   ├── Combat/
│   │   ├── WeaponBase.h/.cpp
│   │   ├── HitDetection.h/.cpp
│   │   ├── DamageTypes.h/.cpp
│   │   └── CombatData.h/.cpp
│   │
│   ├── Abilities/
│   │   ├── AbilityBase.h/.cpp
│   │   ├── AbilityInstance.h/.cpp
│   │   └── AbilityData.h/.cpp
│   │
│   ├── AI/
│   │   ├── EnemyAIController.h/.cpp
│   │   ├── AIBehaviorComponent.h/.cpp
│   │   ├── AICombatDirector.h/.cpp
│   │   └── AIDebugHelpers.h/.cpp
│   │
│   ├── UI/
│   ├── Save/
│   ├── Subsystems/
│   └── Utils/
```

This structure separates **actors**, **systems**, and **data**.

------------------------------------------------------------------------

# Core Gameplay Entities

## Character Layer

### IroncladCharacterBase

Responsibilities: - Attribute ownership\
- Component orchestration\
- Animation binding\
- Damage reception

Does NOT: - Decide combat logic\
- Execute abilities\
- Contain AI behavior

------------------------------------------------------------------------

### PlayerCharacter

Responsibilities: - Input binding\
- Camera integration\
- Player-specific abilities\
- UI event dispatch

------------------------------------------------------------------------

### EnemyCharacterBase

Responsibilities: - AI integration\
- Threat handling\
- Combat feedback

------------------------------------------------------------------------

# Gameplay Components

## AttributeComponent

Owns: - Health\
- Stamina\
- Regeneration\
- Events (OnHealthChanged, OnDeath)

Never: - Plays animations\
- Triggers abilities

------------------------------------------------------------------------

## CombatComponent

Owns: - Weapon handling\
- Attack execution\
- Hit detection\
- Damage application\
- Combat states

Never: - Makes AI decisions\
- Handles UI\
- Stores attributes

------------------------------------------------------------------------

## AbilityComponent

Owns: - Ability registration\
- Cooldowns\
- Resource validation\
- Ability execution flow

Abilities are: - Data-driven\
- Self-contained\
- Reusable

------------------------------------------------------------------------

## TargetingComponent

Owns: - Lock-on logic\
- Target selection\
- Camera assistance\
- Distance/angle queries

------------------------------------------------------------------------

# Combat System Architecture

``` text
Input 
  → AbilityComponent 
      → Ability 
          → CombatComponent 
              → Weapon 
                  → HitDetection 
                      → Damage 
                          → Attributes 
                              → Animation / Feedback
```

Principles: - Abilities trigger combat\
- Combat executes mechanics\
- Characters own state\
- Animation reflects gameplay, not drives logic

------------------------------------------------------------------------

# Ability System Design

## AbilityBase

Responsibilities: - Activation rules\
- Cost checks\
- Cooldowns\
- Execution pipeline

Abilities may: - Spawn hitboxes\
- Apply gameplay effects\
- Trigger animation montages\
- Communicate with CombatComponent

Abilities never: - Directly change UI\
- Hard-code character logic

------------------------------------------------------------------------

# Weapon System

## WeaponBase

Responsibilities: - Weapon data\
- Trace sockets\
- Damage profiles\
- Attack modifiers

Weapons are: - DataAssets + C++ behavior\
- Swappable\
- Combat-agnostic

------------------------------------------------------------------------

# AI Architecture

``` text
EnemyCharacter
   ↓
AIController
   ↓
Behavior Tree / FSM
   ↓
AICombatDirector
   ↓
CombatComponent / Abilities
```

## EnemyAIController

-   Perception\
-   Blackboard\
-   High-level control

## AICombatDirector

-   Attack selection\
-   Cooldown coordination\
-   Spacing logic\
-   Phase control

AI never: - Deals damage directly\
- Accesses animation graph\
- Owns combat execution

------------------------------------------------------------------------

# Animation Integration

Combat is animation-driven but gameplay-authoritative.

-   Montages define timing\
-   Notifies trigger windows\
-   Root motion used for combat\
-   Gameplay code validates state

Animation does not: - Decide hits\
- Own cooldowns\
- Apply damage

------------------------------------------------------------------------

# Data-Driven Design

The following must be data-driven:

-   Weapons\
-   Abilities\
-   Enemies\
-   Attributes\
-   Tunables

Tools: - DataAssets\
- DataTables\
- Gameplay Tags

Goal: zero recompiles for balance changes.

------------------------------------------------------------------------

# UI Architecture

UI only observes.

-   Subscribes to gameplay events\
-   Never drives gameplay logic\
-   Uses view-model style binding

------------------------------------------------------------------------

# Save System

Save system captures:

-   Player attributes\
-   Unlocked abilities\
-   Arena progression

Uses: - SaveGame objects\
- System-level serialization

------------------------------------------------------------------------

# Debug & Development Tools

Planned:

-   On-screen debug HUD\
-   AI state overlays\
-   Combat logging\
-   Ability tracing\
-   Attribute inspectors

Debug must be built into systems, not hacked in later.

------------------------------------------------------------------------

# Performance & Code Standards

-   No gameplay logic in Tick unless justified\
-   Events over polling\
-   Minimal coupling\
-   Clear ownership\
-   Small, focused classes

------------------------------------------------------------------------

# Architecture Success Criteria

This architecture succeeds if:

-   New abilities can be added without modifying character code\
-   New enemies can be added without modifying combat code\
-   AI and combat can evolve independently\
-   Systems are testable and debuggable\
-   Code supports interview-level discussion

------------------------------------------------------------------------

# Living Architecture

This architecture will evolve. Changes must be:

-   Reflected here\
-   Justified technically\
-   Logged in devlogs
