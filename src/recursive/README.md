

- in this part i need to :
  - parse named.root + add ttl logic
  - add selection algorithms logic :
    - Always First: i will use that for testing purpose.
    - Random choice: pick any root server each time you start or query.
    - Round-robin: rotate through the list (A → B → C …).
    - Failover: start with one; if no response, try the next.




## Structure line format:

```
<NAME>   <TTL>   <CLASS>   <TYPE>   <RDATA>
```

---

### 1. **Root zone (`<NAME>` = `.`)**

Example:

```
. 3600000 IN NS A.ROOT-SERVERS.NET.
```

* `.` means **the root zone** (the top of the DNS hierarchy).
* The record says: *“the root zone is served by the name server `A.ROOT-SERVERS.NET.`”*

That’s the **delegation**: it defines which servers hold the root zone.

---

### 2. **NS record target (last field = `<RDATA>`)**

* The last field is the **hostname** of the root name server.
* Example above: `A.ROOT-SERVERS.NET.` is the target name of that `NS` record.
* That hostname must have its **own address records** (`A` and `AAAA`) elsewhere in the file:

  ```
  A.ROOT-SERVERS.NET. 3600000 IN A 198.41.0.4
  A.ROOT-SERVERS.NET. 3600000 IN AAAA 2001:503:ba3e::2:30
  ```

  These are **glue records**—they give the resolver the IPs to reach that server.

---

### 3. **Relation between fields**

Together:

```
. IN NS A.ROOT-SERVERS.NET.
A.ROOT-SERVERS.NET. IN A 198.41.0.4
```

means:

> “To query the root zone (`.`), contact the server named `A.ROOT-SERVERS.NET.`, reachable at IP `198.41.0.4`.”

---

### 4. **TTL consistency**

* TTL values in `named.root` are usually identical (3600000) for simplicity.
* They **don’t have to be equal**; each record type can legally have its own TTL.
* But in practice, IANA sets the same TTL for all root-hint entries for uniform caching.

---

### Summary

| Field                      | Meaning                                   |
| -------------------------- | ----------------------------------------- |
| `.`                        | The root zone (apex of DNS)               |
| `3600000`                  | TTL in seconds                            |
| `IN`                       | Internet class                            |
| `NS`                       | Type of record (delegates zone authority) |
| `A.ROOT-SERVERS.NET.`      | Hostname of the name server (target)      |
| Later `A` / `AAAA` records | Glue addresses for that hostname          |

The `.` line declares **who serves the root zone**, and the `A`/`AAAA` lines give you **how to reach them**.

