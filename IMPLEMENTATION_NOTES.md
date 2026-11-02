# Implementation and maintenance notes

- C++17 offline console app; SQLite runtime database, with prepared SQL statements through `Db`.
- Password hashes use OpenSSL PBKDF2-HMAC-SHA256 with 210,000 iterations and a random 16-byte salt.
- `User` is abstract and role subclasses use inheritance/polymorphism.
- Marking, moderation and release writes use SQLite transactions; CR1 class extension uses one transaction for all eligible enrolled students.
- Late-date and extension rules are isolated in `src/business_rules.cpp` for unit testing and coverage.
- CR1 adds `extension_scope`; existing individual overrides remain unchanged when a class extension is applied.
- First attempts can be late and receive the specified penalty; resubmissions require submission at or before the effective deadline.
- Known deployment limitations are documented in `docs/phase7/RELEASE_NOTES_v1.0.md` and `INSTALLATION_GUIDE.md`.
