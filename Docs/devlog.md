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
Establish a deterministic, maintainable input pipeline using Enhanced Input, removing reliance on template assets and eliminating conflicting mapping sources.

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
