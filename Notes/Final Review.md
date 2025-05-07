Data/Domain Description
- Make semantic models -> ER diagram -> translate to conceptual view
Legacy Data + Domain Analaysis
- Get relations from legacy data, get functional dependencies from domain analysis -> identify keys -> get to BCNF or 3NF -> translate to conceptual view
# Database System
## External Views
- Views
- DB application
	- prepared statement
	- unprepared statement
	- occi / call level interface
## Conceptual Views
- SQL DDL used to make physical view
- 
## Internal/Physical Views
- data types, constraints, sequences, triggers, indexing, procedures

## Protocols
lock based:
- 2PL -> conflict serializable
- strict 2PL -> cascadeless

could lead to a deadlock situation
- detect and recover
- prevent
	- wait-die
	- wait-wait


timestamp based:


optimistic:


buffer manager uses logging
- WAL protocol

