# Implementation notes
- Application is C++17 and local/offline.
- SQLite is used for runtime persistence as required by the Phase 4 brief; the Phase 3 MySQL schema is a design artefact and not the runtime database.
- Database access is encapsulated in `Db`; user input is bound through prepared statements.
- `User` is abstract and four role subclasses implement polymorphic dashboards/role capability.
- Password hashes use OpenSSL PBKDF2-HMAC-SHA256, 210,000 iterations, and a cryptographically random 16-byte salt.
- Marking, moderation, release and audit events use transactions.
- Late penalty is isolated in `adjustedMaximum` (Strategy-style policy function).
- Known limitation: timestamps are stored as local strings; the Phase 2 calendar-day assumption is used. Confirm boundary semantics and final policy with the supervisor.
