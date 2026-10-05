# ⚔️ C++ Smart Pointers Arena

A small C++ project demonstrating **smart pointers, ownership, object lifetime, and automatic memory management** through a simple arena combat simulation.

The project uses:

* `std::unique_ptr`
* `std::shared_ptr`
* `std::weak_ptr`
* `std::move()`
* `weak_ptr::lock()`
* Constructors and destructors
* Automatic RAM cleanup

---

## 🎯 Project Purpose

The main goal of this project is to understand how **C++ smart pointers manage dynamically allocated objects** and how object lifetime changes when pointers are created, moved, shared, or destroyed.

The project simulates a small arena where:

* A `Character` can have a weapon.
* A `Character` can have an active buff.
* A `Character` can target another character.
* The target is stored using a `weak_ptr`.
* When the target is destroyed, the attacker safely detects that the target is no longer available.

---

## 🧩 Classes

### `Weapon`

Represents a weapon used by a character.

It contains:

* Weapon name
* Damage value

Example:

```cpp
Weapon("sword", 35);
```

The weapon is owned by a `Character` through:

```cpp
std::unique_ptr<Weapon>
```

This means that one character has exclusive ownership of its weapon.

---

### `Buff`

Represents a temporary effect applied to a character.

It contains:

* Buff name
* Duration

The buff is managed using:

```cpp
std::shared_ptr<Buff>
```

This allows the same `Buff` object to potentially be shared between multiple owners.

---

### `Character`

Represents an arena character.

Each character contains:

```cpp
std::unique_ptr<Weapon> weapon;
std::shared_ptr<Buff> active_buff;
std::weak_ptr<Character> target;
```

These three smart pointers demonstrate three different ownership models.

---

## 🧠 Smart Pointer Usage

### `std::unique_ptr`

The weapon is stored using:

```cpp
std::unique_ptr<Weapon> weapon;
```

A `unique_ptr` means:

> Only one object owns this resource.

The weapon is transferred to the character with:

```cpp
hero->equip_weapon(std::move(sword));
```

After `std::move(sword)`, the ownership of the weapon is transferred from `sword` to `hero`.

The original `sword` pointer no longer owns the object.

---

### `std::shared_ptr`

Characters are created using:

```cpp
std::shared_ptr<Character> hero(
    new Character("Lakaka", 100)
);

std::shared_ptr<Character> monster(
    new Character("Burger", 50)
);
```

A `shared_ptr` allows multiple pointers to share ownership of the same object.

The `Buff` is also managed using:

```cpp
std::shared_ptr<Buff> shield(
    new Buff("shield", 5)
);
```

It is then assigned to the character:

```cpp
hero->apply_buff(shield);
```

Now both `shield` and `hero->active_buff` refer to the same `Buff`.

---

### `std::weak_ptr`

The target is stored as:

```cpp
std::weak_ptr<Character> target;
```

This is important because the character does **not own** its target.

The target is assigned with:

```cpp
hero->set_target(monster);
```

The `weak_ptr` observes the `monster` without increasing its reference count.

This prevents an ownership cycle.

---

## 🔓 `weak_ptr::lock()`

Before attacking, the program tries to obtain a temporary `shared_ptr`:

```cpp
std::shared_ptr<Character> target_sp = target.lock();
```

If the target still exists:

```cpp
if (target_sp)
```

the attack is performed.

If the target has already been destroyed:

```cpp
else
```

the program prints:

```text
Lakaka has no valid target!
```

This demonstrates how `weak_ptr` can safely check whether an object still exists.

---

## 💥 Target Destruction

The monster is explicitly destroyed with:

```cpp
monster.reset();
```

At this point, the `monster` object is destroyed because there are no other `shared_ptr`s owning it.

However, `hero->target` is only a `weak_ptr`.

Therefore, it does not keep the monster alive.

When the hero attacks again:

```cpp
hero->attack();
```

`target.lock()` fails and returns an empty `shared_ptr`.

The program safely detects that the target no longer exists.

---

## 🧹 Automatic Memory Cleanup

One of the main advantages of smart pointers is automatic memory management.

When objects are no longer needed, their destructors are automatically called.

For example:

```cpp
~Weapon()
```

prints:

```text
Weapon - sword destroyed
```

The `Character` destructor prints:

```text
Character - Lakaka died from arena
```

The `Buff` destructor prints:

```text
Buff - shield expired and destroyed
```

This allows us to observe exactly when each object leaves memory.

---

## ⚔️ Program Flow

The program works approximately like this:

```text
ARENA INITIALIZATION
        |
        v
Create Hero
        |
        v
Create Monster
        |
        v
Create Sword
        |
        v
Move Sword -> Hero
        |
        v
Create Shield Buff
        |
        v
Hero receives Buff
        |
        v
Hero targets Monster
        |
        v
COMBAT BEGINS
        |
        v
Hero attacks Monster
        |
        v
Monster is destroyed
        |
        v
Hero attacks again
        |
        v
Target is invalid
        |
        v
Automatic cleanup
```

---

## 🖥️ Example Output

A possible output is:

```text
=== ARENA INITIALIZATION ===
Character - Lakaka spawned with 100 HP
Character - Burger spawned with 50 HP
Weapon - sword created - 35
Buff - shield created - duration: 5

=== COMBAT BEGINS ===
Lakaka attack Burger with sword for 35 dmg!

=== MONSTER DIES (RAM Cleanup) ===
Character - Burger died from arena

=== ATTACK AFTER TARGET DEATH ===
Lakaka has no valid target!

Character - Lakaka died from arena
Buff - shield expired and destroyed
Weapon - sword destroyed
```

The exact destruction order can depend on the lifetime and ownership of the objects.

---

## 📚 Concepts Demonstrated

This project demonstrates several important C++ concepts:

| Concept            | Usage                                         |
| ------------------ | --------------------------------------------- |
| `unique_ptr`       | Exclusive ownership of the weapon             |
| `shared_ptr`       | Shared ownership of characters and buffs      |
| `weak_ptr`         | Non-owning reference to a target              |
| `std::move()`      | Transfer of ownership                         |
| `lock()`           | Safely accessing an object through `weak_ptr` |
| Constructor        | Object initialization                         |
| Destructor         | Object cleanup                                |
| RAII               | Automatic resource management                 |
| Dynamic memory     | Objects created on the heap                   |
| Reference counting | `shared_ptr` ownership management             |

---

## 🚀 How to Compile

Compile the project with:

```bash
g++ main.cpp -std=c++17 -o arena
```

Run:

```bash
./arena
```

---

## 🛠️ Technologies

* **Language:** C++
* **Standard:** C++17
* **Compiler:** g++
* **Concepts:** OOP, Smart Pointers, RAII, Dynamic Memory Management

---

## 🎓 What I Learned

Through this project I practiced:

* How `unique_ptr` provides exclusive ownership.
* How `shared_ptr` manages shared ownership.
* Why `weak_ptr` does not increase the reference count.
* How `std::move()` transfers ownership.
* How `weak_ptr::lock()` safely checks whether an object still exists.
* How constructors and destructors show object lifetime.
* How smart pointers help prevent memory leaks.
* How C++ automatically cleans up resources using RAII.

---

## 📌 Future Improvements

Possible improvements for the project:

* Add HP reduction when characters attack.
* Add multiple weapons.
* Add different types of buffs.
* Add buff expiration based on duration.
* Add a real combat system.
* Add critical hits and defense.
* Add multiple characters and targets.
* Add a simple game loop.

---

## 👨‍💻 Project

This project was created as a C++ practice project focused on **Smart Pointers and Memory Management**.
