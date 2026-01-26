# Project Ironclad — Combat Architecture

## Purpose

This document describes the design and implementation of the combat system in Project Ironclad as an engineering artifact.  
It explains how player input becomes validated combat actions, how animation drives gameplay timing, and how damage is applied to enemies.

This is intended both as:
- Internal technical documentation
- A portfolio-facing explanation of the combat framework

---

## Scope

Covered in this document:
- Combat state model and transitions
- Action validation (state, stamina, cooldown)
- Weapon and attack data (data-driven design)
- Animation-driven hit detection
- Damage pipeline
- Dummy enemy integration (test harness)

Not covered here:
- Hit reactions and death presentation logic
- Advanced lock-on internals (beyond combat usage)
- Full ability system expansion (beyond current cooldown + stamina abilities)

---

## High-Level Flow

### 1. Input → Combat Intent

Player input generates a combat intent:
- Light Attack
- Heavy Attack
- Dodge
- Ability (e.g. Stamina Burst)

All intents pass through a single validation and execution pipeline, rather than being handled ad-hoc per action.

---

### 2. Validation Layer (CombatGate)

Before any combat action is executed, it must be accepted by the CombatGate.

CombatGate validates:
- Current combat state
- Stamina availability and cost
- Cooldowns for ability-type actions

Only accepted actions can proceed to animation and gameplay execution.

---

### 3. Animation-Driven Execution

Combat actions are executed through animation montages.

Gameplay effects (hit detection) are synchronized with animation using notify states, ensuring that:
- Damage only occurs during visually valid frames
- Timing remains consistent across framerate and latency variations

---

### 4. Hit Detection → Damage Pipeline

During active hit windows:
- Hit detection runs traces or overlaps
- Hits generate events consumed by the damage system
- Damage is applied to the target (dummy enemy)

---

## Combat State Model

### State Definitions (current milestone)

Minimal state set used in the current combat milestone:

- **Idle**  
  Player can accept new actions (attack, dodge, ability).

- **Attacking**  
  An attack montage is playing and hit windows may be active.

- **Recovery**  
  Post-attack cooldown window where some actions are blocked.

- **Dodging**  
  Player is executing a dodge, consuming stamina and temporarily altering movement/combat availability.

- **Dead / Stunned** (if implemented)  
  Terminal or blocking states that deny most actions.

---

### State Transitions

- Idle → Attacking  
- Attacking → Recovery  
- Recovery → Idle  
- Idle / Recovery → Dodging  
- Dodging → Idle  
- Any → Idle (emergency reset)

---

## Weapon and Attack Data

Combat is driven primarily by data assets rather than hard-coded values.

### Weapon Data
- Weapon type
- Base damage scalar
- Trace profile
- Socket references

### Attack Step Data
- Montage reference
- Stamina cost
- Damage tags
- Hit window notify identifiers

---

## Action Validation (CombatGate)

All combat actions pass through CombatGate before execution.

Validation order:
1. State gating
2. Stamina check
3. Cooldown check (abilities)

---

## Animation-Driven Hit Detection

Hit detection is active only during explicit notify states inside animation montages.

This ensures damage only occurs when visually appropriate.

---

## Damage Pipeline

Stages:
1. Hit event emitted
2. Damage applied
3. Death condition checked

---

## Dummy Enemy (Test Harness)

A simple enemy validates:
- Hit detection
- Damage application
- Kill condition

---

## Technical Breakdown

### What Was Built
- Animation-driven melee combat
- Centralized CombatGate
- Modular hit detection
- Decoupled damage pipeline
- Dummy enemy test harness

### Why It Matters
- Syncs gameplay with animation
- Prevents invalid actions
- Data-driven extensibility
- Debuggable pipeline

---

## Showcase Clips

Recommended:
- Combo clip
- Dodge with stamina
- Lock-on + kill

---

## Proof Checklist

- Logs validate actions
- Stamina only spent on accepted actions
- Hit windows via notifies
- Damage reduces health
- Kill condition triggered
