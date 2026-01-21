# Development Log

## Project Ironclad

------------------------------------------------------------------------

# Purpose of This Document

This document serves as the development journal for Project Ironclad. It
records daily and weekly progress, technical decisions, problems
encountered, and solutions implemented.

This devlog is both: - A production tracking tool\
- A portfolio artifact demonstrating engineering process, iteration, and
problem-solving

All major architectural changes, system designs, and scope decisions
should be logged here.

------------------------------------------------------------------------

# How to Use This Devlog

Each entry should include:

-   **Date / Sprint / Milestone**
-   **Goals for the session**
-   **What was completed**
-   **Technical details**
-   **Problems encountered**
-   **Solutions / Decisions**
-   **Next actions**

This format supports: - Clear iteration history\
- Interview discussion material\
- Portfolio write-ups

------------------------------------------------------------------------

# Development Workflow

The project follows a Kanban workflow using GitHub Projects:

1.  Backlog is reviewed at the start of each session\
2.  Tasks are moved to *In Progress*\
3.  Completed tasks are moved to *Done*\
4.  This devlog is updated to reflect work performed

Each devlog entry should reference the corresponding tasks when
applicable.

------------------------------------------------------------------------

# Entry Template

## \[YYYY-MM-DD\] -- Phase / Sprint

## \### Session Goals

## \### Work Completed

## \### Technical Notes

## \### Problems Encountered

## \### Solutions / Decisions

## \### Next Actions

------------------------------------------------------------------------

# Log Entries

## \[2026-01-09\] -- Project Initialization

### Session Goals
-   Set up repository structure
-   Create documentation framework

### Work Completed
-   Created README.md
-   Added design.md
-   Added milestones.md
-   Added architecture.md
-   Established project structure

### Technical Notes
-   Defined high-level gameplay architecture
-   Established documentation standards for portfolio

### Problems Encountered
-   None

### Solutions / Decisions
-   Adopted documentation-first workflow
-   Committed pre-production artifacts before gameplay code

### Next Actions
-   Create Unreal Engine 5 C++ project
-   Configure .gitignore and Git LFS
-   Begin Phase 1: Foundation

## \[2026-01-10\] -- Phase 1: Foundation

### Session Goals
-   Establish base character architecture in C++
-   Replace template behavior with Ironclad character classes
-   Restore full player control (move, look, jump) using Enhanced Input
-   Integrate a character mesh and animations for a presentable third-person prototype

### Work Completed
-   Created 'AIroncladCharacterBase' as the foundational gameplay character class
-   Implemented third-person camera components in the base class (SpringArm + FollowCamera)
-   Created 'AIroncladPlayerCharacter' deriving from the base class
-   Wired Enhanced Input in C++ for movement, camera look, and jump
-   Created/updated 'BP_IroncladPlayerCharacter' to assign input assets and character mesh/animation setup
-   Set Default Pawn Class in Maps & Modes to the Blueprint player and verified PIE control

### Technical Notes
-   Input architecture follows a “C++ behavior, Blueprint data” pattern:
    -   C++ binds actions and applies mapping contexts
    -   Blueprint assigns Mapping Context and Input Actions (including selecting the correct look action asset)
-   Camera system is centralized in 'AIroncladCharacterBase' to ensure consistent third-person behavior across derived characters
-   Template input assets were reused rather than recreated to preserve known-good mappings

### Problems Encountered
-   Visual Studio showed persistent IntelliSense errors despite successful Unreal/UBT builds
-   Player pawn spawned successfully but initially had no movement/camera control after replacing the template character
-   Mouse look did not work until the correct input action asset was assigned

### Solutions / Decisions
-   Treated IntelliSense-only errors as non-blocking once UBT builds succeeded; filtered to Build errors for signal clarity
-   Implemented Enhanced Input binding in 'AIroncladPlayerCharacter' and assigned assets through Blueprint defaults
-   Standardized third-person camera by adding SpringArm + FollowCamera to 'AIroncladCharacterBase'
-   Reused template Mapping Context and Input Actions; selected 'IA_MouseLook' for mouse camera control

### Next Actions
-   Clean up duplicate/unused input assets (if any) to reduce confusion

## \[2026-01-11\] — Phase 1 Foundation - Input Asset Cleanup & Standardization

### Session Goals 
- Establish a deterministic, maintainable input pipeline using Enhanced Input, removing reliance on template assets and eliminating conflicting mapping sources.

### Work Completed
- Created project-owned input assets ('IMC_Ironclad_Gameplay', 'IA_Ironclad_*') and migrated all bindings.
- Centralized mapping context application in 'AIroncladPlayerCharacter' (C++) to make the pawn the single authority.
- Cleared template PlayerController mapping contexts to prevent duplicate/overriding input sources.
- Resolved 'AddMappingContext(nullptr)' warnings by removing empty controller array entries.
- Updated dependent blueprints (including early combat-related assets) to reference Ironclad input assets.
- Validated C++ bindings for Move, Look, and Jump; confirmed camera control and movement behavior across maps.

### Technical Notes
- Enhanced Input is now deterministic: one IMC, one owner (the pawn), explicit bindings.
- This structure supports future layering (combat/UI/debug contexts) without reintroducing template conflicts.

### Problems Encountered
- None

### Solutions / Decisions
- Input system is standardized, predictable, and portfolio-clean. Ready for combat input expansion without hidden template dependencies.

### Next Actions
- Proceed to base chracter architeture that will support combat, AI, and abilities later without refactors.

## \[2026-01-12\] — Phase 1 Foundation - Camera, Vitals and Sprint Animation

### Session Goals
- Implement a God of War–style third-person camera system.
- Build a reusable Health & Stamina system.
- Integrate base locomotion animations using the Kwang character.
- Establish clean architectural boundaries between gameplay systems and presentation.

### Work Completed
- Implemented a player-only third-person camera using a spring arm with cinematic framing.
- Fixed camera pitch and rotation issues by properly creating camera components in the player class.
- Built a reusable Vitals Component for Health and Stamina and attached it to the base character class.
- Routed Unreal’s TakeDamage() pipeline into the Vitals system.
- Implemented death handling (movement disabled at zero health).
- Added stamina usage with sprint gating and regeneration.
- Integrated the Kwang character from Unreal Fab as the player mesh.
- Created and wired a sprint animation state in Kwang’s locomotion state machine.
- Verified sprint animation playback and stamina drain in PIE.
- Completed three core Phase 1 cards:
    - God of War–Style Camera System
    - Health & Stamina Systems
    - Base Animation Setup

### Technical Notes
- Camera is owned exclusively by AIroncladPlayerCharacter, keeping AIroncladCharacterBase clean for enemies/NPCs.
- Health and stamina are implemented via a dedicated UIroncladVitalsComponent, enabling reuse across all characters.
- Damage is routed through TakeDamage() into the Vitals system.
- Sprint consumes stamina over time and auto-stops when depleted.
- Kwang animations are integrated via a locomotion state machine with an explicit Sprint state.
- Gameplay state (sprint) is cleanly decoupled from animation logic.

### Problems Encountered
- Camera pitch initially did not respond due to improper component creation and Blueprint overrides.
- Movement inputs behaved incorrectly due to Enhanced Input mapping misconfiguration.
- Sprint animation did not trigger at first because Kwang’s locomotion was consolidated into a single state.
- Difficulty locating inherited animation variables due to unfamiliarity with AnimBP class hierarchy.

### Solutions / Decisions
- Corrected Enhanced Input axis mappings and removed conflicting modifiers.
- Added a dedicated Sprint state inside the locomotion state machine to explicitly handle sprint animation.
- Used a thin gameplay-to-animation integration approach to avoid rewriting vendor animation logic.
- Accepted current “battle-ready vs exploration” stance differences in Kwang’s animations, with a plan to address them later via gameplay modes (exploration vs combat).

### Next Actions
- Create the First Arena Test Level to provide a controlled environment for combat and AI testing.
- Begin preparing for Phase 2 (Core Combat):
    - Weapon handling
    - Attack input
    - Hit detection and damage application
- Plan future animation work for:
    - Exploration vs Combat locomotion modes
    - Turn-in-place and lock-on systems
    - Upper-body overlays for combat stances

## \[2026-01-13\] — Phase 1 Foundation

### Session Goals 
- Create a dedicated arena test level to validate core systems outside the template map.
- Build a minimal arena blockout to test camera collision/framing and movement at scale.
- Add lightweight test harness markers to support upcoming combat and AI work.

### Work Completed
- Created L_ArenaTest and confirmed correct pawn spawn and input parity with Lvl_ThirdPerson.
- Blocked out arena floor and boundaries to constrain movement and test camera behavior.
- Added named spawn/reference TargetPoints to standardize future testing workflows.

### Technical Notes
- Level behavior matched the template map via existing project GameMode/Pawn defaults (no per-level overrides required).
- Arena blockout kept intentionally minimal to prioritize camera occlusion/collision validation and clean capture conditions.
- Spawn markers established consistent spatial anchors for future lock-on, hit detection, and dummy placement.

### Problems Encountered
- None

### Solutions / Decisions
- Treat L_ArenaTest as the primary Core Combat test harness going forward (minimal geometry + named reference points).

### Next Actions
- Begin Phase 2 (Core Combat) with Lock-On Camera System:
    - Implement target acquisition + lock toggle input.
    - Add camera tracking behavior and clear release rules.

## \[2026-01-14\] -- Phase 2 / Lock-On Camera System

### Session Goals
- Complete lock-on camera system.
- Add validation, auto-unlock, and debugging tools.
- Ensure system is stable and testable with a dummy target.

### Work Completed
- Finalized lock-on toggle, targeting, and camera framing.
- Implemented auto-unlock rules (distance, line-of-sight with grace).
- Added debug visualization and on-screen diagnostics.
- Resolved input conflicts and verified behavior in arena test level.

### Technical Notes
- Target acquisition uses sphere overlap with tag-based filtering.
- Lock-on validity centralized with timers to prevent flicker.
- Camera behavior driven by Tick with smooth interpolation.
- Debug tools gated behind input toggle and build-safe drawing.

### Problems Encountered
- Debug input conflicted with editor controls (F8, `/`).
- Early Tick returns previously blocked camera updates outside sprint.

### Solutions / Decisions
- Rebound debug toggle to a non-conflicting key (`P`).
- Restructured Tick to separate sprint logic from camera/lock-on updates.
- Kept tag-based targets for testing; class-based enemies will replace this later.

### Next Actions
- Proceed with Phase 2 combat features (weapon system foundation).
- Integrate lock-on with upcoming enemy base class.

## \[2026-01-15\] — Phase 2 / Core Combat

### Session Goals
- Implement a data-driven weapon foundation.
- Spawn a default weapon and support basic weapon swapping.

### Work Completed
- Created Weapon DataAsset type for tunable combat parameters.
- Implemented weapon actor and weapon component with equip/unequip.
- Configured player to spawn with a default weapon from DataAsset.
- Added minimal loadout cycling to swap between sword and axe.

### Technical Notes
- Refactored weapon actor to use a SceneComponent root so mesh offsets can be adjusted per weapon in Blueprint.
- Attachment is socket-driven; per-weapon alignment is handled via mesh relative transforms inside the weapon actor BP.
- Weapon swapping replaces the equipped actor cleanly, keeping the component as the single source of truth.

### Problems Encountered
- Weapon orientation was locked due to inherited root.
- Attachment appeared incorrect after root change.

### Solutions / Decisions
- Introduced a SceneComponent root in the weapon actor to enable local mesh rotation/offset.
- Ensured attachment targets the character’s primary skeletal mesh and validated socket usage.
- Kept weapon definitions fully data-driven to support future tuning and additional weapons.

### Next Actions
- Begin Card 2.4: integrate weapon data with attack execution (light/heavy) via animation/montages.
- Wire combat gate to weapon data (costs, montages) and add state exit via animation events.

## \[2026-01-16\] — Phase 2 / Core Combat

### Session Goals
- Complete Card 2.4: animation-driven light attack with proper state exit.

### Work Completed
- Light attack montage playback integrated into the combat flow.
- Hit window implemented via NotifyState with open/close callbacks.
- Attacking state now exits automatically on montage blend-out/end.
- Recovery timer returns the character to Idle without player input.

### Technical Notes
- Added a no-cost state transition path for system-driven changes (distinct from action acceptance).
- Recovery is idempotent to handle both blend-out and montage end safely.

### Problems Encountered
- Linker errors due to missing/mismatched state transition symbols.
- Core type resolution issues when using `TCHAR` in component interfaces.

### Solutions / Decisions
- Implemented a dedicated state setter for montage/system transitions.
- Replaced `TCHAR*` debug labels with `FName` for robustness and cleaner headers.

### Next Actions
- Begin Card 2.5: combo/chain support (light → light), using montage windows for input buffering.

## \[2026-01-17\] — Phase 2 / Core Combat

### Session Goals
- Generalize attack lifecycle for Light and Heavy attacks
- Ensure animation-driven recovery and correct combat state transitions

### Work Completed
- Implemented a shared attack execution pipeline using a unified attack context
- Routed Light and Heavy attacks through the same montage-driven lifecycle
- Fixed montage delegate binding order to ensure blend-out/end callbacks fire
- Restored proper transition from Attacking → Recovering → Idle

### Technical Notes
- Montage delegates must be bound *after* Montage_Play, as the montage instance does not exist beforehand
- AnimNotifies confirmed montage playback and animation evaluation
- CombatGate state issues were caused by recovery never triggering, not by tuning or gate logic

### Problems Encountered
- Attack recovery never started after the first attack
- Combat state remained stuck in Attacking, causing further inputs to be rejected
- Montage blend-out and end callbacks were not firing

### Solutions / Decisions
- Identified delegate binding order as the root cause
- Reordered delegate binding to occur after Montage_Play
- Confirmed fix via logging and PIE testing
- Locked in a unified, extensible attack lifecycle for future combo work

### Next Actions
- Implement Card 2.6 — Combo Handling (First Pass)

## \[2026-01-19\] — Phase 2 / Core Combat

### Session Goals
- Get Card 2.6 combo chaining working end-to-end (buffering, deterministic steps, reset rules).
- Validate animation-authored chain windows and combat-state integration.

### Work Completed
- Wired UIroncladComboComponent into the player attack input flow and verified multi-step chaining (now supports >2 steps via DataAsset).
- Implemented/reset combo lifecycle so new attack sequences reliably restart from Step 0 after returning to Idle.

### Technical Notes
- Combo chaining is driven by UAnimNotifyState_IroncladComboWindow (authoring windows per montage controls timing deterministically).
- Input routing split:
    - Idle → CombatGate RequestAction (stamina + state entry) → Combo start
    - Attacking → Combo only (avoids CombatGate rejection while already attacking)
- Combo reset is bound to CombatGate state transitions (dynamic multicast delegate binding) to reset on Idle and interruption states.

### Problems Encountered
- Combo chain input was rejected by CombatGate while in Attacking, preventing the second press from reaching the combo system.
- Combo step index persisted across combat sessions, preventing subsequent attacks after returning to Idle.
- Combo debug logs were initially hidden due to Verbose logging level.

### Solutions / Decisions
- Bypassed CombatGate authorization for chain inputs while in Attacking; gate only the initial entry from Idle.
- Reset combo when CombatGate transitions to Idle (and on Dodging/Stunned) via combat state change delegate.
- Promoted combo logs to visible levels during bring-up to validate step transitions and window timing.

### Next Actions
- Start Card 2.7 — Dodge With Stamina Cost:
    - Add/validate stamina charge on dodge request
    - Ensure dodge interrupts attacks/combos deterministically (state + montage handling)
    - Confirm recovery/lockouts and reset rules remain consistent with CombatGate authority

## \[2026-01-20\] — Phase 2 / Core Combat

### Session Goals
- Implement dodge as a stamina-gated combat action with invulnerability window and animation-driven execution.

### Work Completed
- Implemented Dodge action through CombatGate with stamina validation and state transition.
- Added dodge montage playback with launch-based movement impulse.
- Integrated full-body montage slot into AnimGraph to support dodge animation.
- Added cooldown, recovery, and fallback safety logic.

### Technical Notes
- Dodge execution triggered via OnCombatActionAccepted delegate (action-driven, not state-driven).
- Recovery timing derived from montage duration; fallback timer cleared on montage end.
- Temporary braking/friction reduction used to preserve impulse distance.
- Full-body slot inserted before IK stage to support montage blending.

### Problems Encountered
- Dodge montage playing but not visible due to missing full-body slot in AnimGraph.
- Recovery timer ending dodge state before montage completion.

### Solutions / Decisions
- Added DefaultGroup.DefaultSlot node on final pose path in AnimGraph.
- Switched recovery timing to montage-derived duration and prioritized montage end delegate.
- Kept time-based i-frame window as first-pass solution (future notify-state upgrade planned).

### Next Actions
- Begin Card 2.8 — Hit Detection System
- Define hitbox / trace strategy for weapon-based attacks

## \[2026-01-21\] — Phase 2 / Core Combat

### Session Goals
- Finalize trace-based hit detection and validate it in gameplay
- Implement a modular damage pipeline decoupled from hit detection
- Create a test enemy to support damage validation and upcoming combat features

### Work Completed
- Validated trace-based hit detection with runtime hit logs and stamina interaction
- Implemented a modular damage pipeline using FDamageSpec and IDamageable
- Integrated damage application into weapon hit events
- Created a dummy enemy class to validate damage flow and health reduction in PIE

### Technical Notes
- Hit detection operates strictly during animation-defined hit windows using trace sweeps
- Damage is applied uniformly via IIroncladDamageable, ensuring player and enemies share the same contract
- Weapon actor builds FDamageSpec from weapon data and hit context
- Damage receiver component manages health state and logs damage for debugging
- Source attribution and health deltas were validated through runtime logs

### Problems Encountered
- No enemy target initially existed to validate the damage pipeline
- Unable to observe full damage flow until a proper receiver actor was in place

### Solutions / Decisions
- Introduced a minimal dummy enemy implementing IDamageable to serve as a test harness
- Decided to keep hit detection, damage calculation, and damage application fully decoupled
- Confirmed the damage pipeline design will support upcoming hit reactions and death logic without refactor

### Next Actions
- Add hit reactions based on damage type and hit context (Card 2.10)
- Implement death logic and visual/audio feedback on zero health
- Ensure GameplayTags flow from weapon data into damage specs for semantic classification

------------------------------------------------------------------------

# Best Practices

-   Keep entries concise but technical\
-   Document *why* decisions were made\
-   Capture architectural changes immediately\
-   Include performance considerations when relevant\
-   Reference specific systems, classes, or files when possible

------------------------------------------------------------------------

# Portfolio Usage

This devlog will later be used to: - Write postmortems\
- Prepare technical interviews\
- Populate portfolio case studies\
- Demonstrate professional development habits

------------------------------------------------------------------------

# Living Document

This document is continuously updated throughout development. It
reflects the evolution of Project Ironclad in real time.
