# Contributing

### Naming
* Namespaces, Classes, Functions, Variables: `low_case`
* Macros: `UPPER_CASE`

### Braces
Opening brace is placed on the same line, that is, there is _no_ line break
before it. If a control flow body consists of a single statement, braces are
omitted.

### Indentation
Use 4 spaces for indentation, not tabs.

### Structure
Following `else`- and `else if`-statements begin on the same line as the
previous closing brace, or a new line if the previous branch contained a single
statement.

### Access specifiers
A class should only declare one specifier per specifier type.

### Documentation
All non-private members need to be documented using Doxygen-style comments.
For functions and methods, parameters are documented first and return type next.

### Example
```cpp
/// A little fish.
struct fish {
    /// Name of the fish.
    const char* name;
    
    // Number of friends.
    unsigned int friend_count;
}

/**
 * Check whether fish is happy.
 * @param fish The fish to check.
 * @return Whether fish is happy.
 */
bool is_happy(const fish& fish);

#include "example.hpp"

bool is_happy(const fish& fish) {
    return fish.friend_count > 0;
}

```