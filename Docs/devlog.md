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
