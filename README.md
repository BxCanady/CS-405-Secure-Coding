# CS-405-Secure-Coding
Secure Coding principles, vulnerability analysis, and best practices for developing secure software applications.
CS 405: Secure Coding — Course Reflection
Brandon Canady
Southern New Hampshire University | B.S. Computer Science, Software Engineering
Key takeaway: Security belongs in the design and development process—not just in a final review.

#How My Approach to Security Changed
CS 405 changed the way I define a successful software project. Before this course, I often thought first about whether a program worked as intended. Now, I also ask what happens when someone enters unexpected data, reaches a resource they should not access, or takes advantage of a mistake in the code. Secure coding is not something I want to add at the end. It should influence design decisions, development, testing, and maintenance from the beginning.
Applying Secure Coding in C++
The hands-on activities made that shift practical. While working with input validation and buffer overflow prevention, I saw why developers must check lengths and boundaries before using data. I also practiced exception handling and learned that assertions should not replace proper runtime error handling. In my Google Test project, I tested string index safety using std::string::at(). Three tests covered valid positions, and three checked invalid positions, including an empty string. All six passed with zero failures. That result confirms the expected behavior of those specific tests, not that every part of an application is secure.
Security Requires More Than One Tool
Using Visual Studio and Cppcheck taught me how static analysis flags possible issues before release. Those warnings still require developer review. My encryption exercise reinforced another distinction: XOR helps demonstrate reversible transformations, but it cannot protect sensitive production data on its own. Defense in depth needs overlapping layers of safe code, access controls, appropriate encryption, testing, and monitoring.
Securing Every Entry Point
One principle I want to carry forward is zero trust. I picture an application as a building with many doors. Protecting the front entrance means little if a side door is left unlocked. In software, those doors include forms, APIs, file uploads, administrative features, and service connections. I want to identify each entry point, validate its inputs, check permissions on the server, and give users only the access they need. That thinking shaped my Green Pace security policy and its focus on layered controls and risk-based priorities.
What I Will Do Next
Going forward, I plan to include threat modeling during design, write positive and negative tests for boundary conditions, review compiler and static-analysis warnings, and automate these checks in a development pipeline. I also want to learn AddressSanitizer and UndefinedBehaviorSanitizer to strengthen future testing. My biggest takeaway from CS 405 is that quality and security belong together: a feature is not truly finished until I have considered how it could fail, who can reach it, and how those risks will be controlled.
Evidence and Next Steps
- Verified: Six Google Test cases for std::string::at() string index safety; 6 passed, 0 failed (three valid-index cases and three invalid-index cases).
- Practiced: C++ input validation, bounds checks, exception handling, static analysis using Visual Studio and Cppcheck, and an educational XOR encryption exercise.
- Designed: A Green Pace security policy addressing defense in depth, zero trust, coding standards, encryption, and authentication/authorization/accounting.
- Next: Add automated CI checks, sanitizer-based testing, deeper authorization tests, and repeatable threat modeling.
