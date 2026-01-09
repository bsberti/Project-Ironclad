# Development Milestones  
## Project Ironclad

---

# Purpose of This Document

This document defines the production roadmap for Project Ironclad. It exists to:

- Establish clear development phases  
- Control scope and technical risk  
- Guide weekly priorities  
- Provide visible production structure  
- Support devlogs and portfolio presentation  

This project follows a milestone-based development model similar to small studio productions.

---

# High-Level Roadmap

The project is structured into five main phases:

1. Foundation  
2. Core Combat  
3. Enemy AI & Interaction  
4. Game Features & Polish  
5. Presentation & Portfolio Packaging  

Each phase produces concrete portfolio artifacts.

---

# Phase 1 – Foundation (Days 1–10)

### Objective  
Create a stable Unreal Engine C++ project with a clean architecture and a playable third-person base.

### Deliverables  
- Unreal Engine 5 C++ project initialized  
- Repository structure finalized  
- Custom player character class  
- Camera system (God of War–style framing)  
- Enhanced Input fully configured  
- Health and stamina systems  
- Base animation setup  
- First arena test level  

### Technical focus  
- Project module layout  
- Component-driven gameplay design  
- Coding standards and naming  
- Git workflow discipline  

### Portfolio outcomes  
- First devlog entry  
- Short video of movement and camera  
- Initial architecture notes  

### Exit criteria  
- Player can move, rotate, and control camera  
- Health and stamina function  
- Project builds cleanly  
- Codebase is organized and documented  

---

# Phase 2 – Core Combat (Days 11–25)

### Objective  
Implement the full player combat framework.

### Deliverables  
- Weapon system foundation  
- Light and heavy attacks  
- Combo handling  
- Dodge with stamina cost  
- Hit detection system  
- Damage pipeline  
- Hit reactions and death logic  
- Ability framework (first pass)  

### Technical focus  
- Combat state management  
- Ability execution flow  
- Animation/gameplay synchronization  
- Data-driven tuning (DataAssets)  

### Portfolio outcomes  
- Combat showcase clips  
- Combat architecture documentation  
- First technical breakdowns  

### Exit criteria  
- Player can fight dummy enemies  
- Attacks are animation-driven  
- Damage system is modular  
- Combat code is documented  

---

# Phase 3 – Enemy AI & Interaction (Days 26–40)

### Objective  
Turn the prototype into a “game” through enemy behavior.

### Deliverables  
- Base enemy C++ class  
- AI perception  
- Decision system (Behavior Tree or FSM)  
- Enemy Type A – Aggressor  
- Enemy Type B – Controller  
- Enemy combat integration  
- Reactions, stagger, death  
- AI debug tools  

### Technical focus  
- AI modularity  
- Blackboard and behavior structure  
- Separation of combat and AI logic  
- Debug and tuning tools  

### Portfolio outcomes  
- AI behavior demo video  
- AI system diagrams  
- Enemy design breakdown  

### Exit criteria  
- Enemies detect, pursue, and attack player  
- Distinct enemy behaviors exist  
- AI is debuggable and extensible  
- Combat encounters are functional  

---

# Phase 4 – Game Features & Polish (Days 41–55)

### Objective  
Reach recruiter-ready quality.

### Deliverables  
- HUD (health, stamina, cooldowns)  
- Lock-on system  
- Save/load  
- Pause and settings menu  
- Visual feedback (VFX, audio)  
- Arena dressing and flow  
- Code cleanup and refactors  

### Technical focus  
- Subsystems  
- UX clarity  
- Performance stability  
- Bug fixing and tuning  

### Portfolio outcomes  
- First public playable build  
- Gameplay trailer capture  
- Final system screenshots  

### Exit criteria  
- Full gameplay loop exists  
- Systems feel cohesive  
- Build is stable  
- Code is presentable  

---

# Phase 5 – Presentation & Packaging (Days 56–60)

### Objective  
Transform the prototype into a professional portfolio piece.

### Deliverables  
- Clean GitHub repository  
- README update  
- Architecture documentation  
- Gameplay trailer (60–90s)  
- Windows build package  
- Devlog consolidation  

### Technical focus  
- Code readability  
- Documentation quality  
- Recruiter-facing presentation  

### Portfolio outcomes  
- Flagship portfolio project  
- Resume bullet integration  
- Shareable project page  

### Exit criteria  
- Project is downloadable  
- Systems are documented  
- Presentation material exists  
- Portfolio-ready state achieved  

---

# Weekly Rhythm

Each week should produce:

- At least one visible gameplay improvement  
- At least one devlog update  
- Clean, incremental commits  
- Updated documentation where applicable  

---

# Risk Management

### High-risk systems  
- Combat feel  
- AI behavior  
- Animation integration  
- Code architecture  

These should be built early and iterated continuously.

### Low-risk systems  
- UI  
- Save/load  
- Menus  
- Presentation  

These should be scheduled after gameplay stabilizes.

---

# Scope Control Rules

- No new core systems after Phase 3  
- Every feature must support portfolio goals  
- No asset-heavy distractions  
- Technical depth > content volume  

---

# Definition of Done

Project Ironclad is considered complete when:

- A full combat loop exists  
- Two enemy archetypes are functional  
- Systems are modular and documented  
- A playable build is available  
- The project clearly supports gameplay programming interviews  

---

# Living Roadmap

This roadmap is expected to evolve. Changes must be:

- Documented  
- Justified  
- Reflected in devlogs  
