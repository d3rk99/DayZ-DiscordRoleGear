# DayZ Discord Role Gear

## Overview
Discord Role Gear is a DayZ server-side mod that talks to the [UniversalApi](https://github.com/DaemonForge/UniversalApi) framework to look up the Discord roles that belong to each connecting player and then equips them with the items you configure for those roles. The mod registers UniversalApi as a dependency, loads game/world/mission scripts, and bootstraps a reusable configuration object on mission start so the gear rules are available everywhere in the mission logic.【F:DiscordRoleGear/config.cpp†L1-L50】【F:DiscordRoleGear/scripts/5_Mission/MissionServer.c†L1-L10】【F:DiscordRoleGear/scripts/5_Mission/DiscordRoleGear.c†L79-L85】

When a client connects, the mission server asks UniversalApi for the player's Discord profile and caches the response. Once you call `HandleDiscordGear` for that player (e.g., after their character is spawned), the mod matches the player's Discord role IDs against the configured gear sets and spawns the correct item (or a random fallback) with the health, quantity, and attachments you specified.【F:DiscordRoleGear/scripts/5_Mission/MissionServer.c†L3-L52】【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L3-L92】

## Features
- **Discord-aware loadouts** – `MissionServer` calls `UApi().ds().GetUser(...)` during `OnClientPrepareEvent` so that every player has their Discord roles on file before they enter the world.【F:DiscordRoleGear/scripts/5_Mission/MissionServer.c†L24-L37】
- **Configurable gear sets** – `DiscordRoleGearConfig` maintains an array of `DGGearSets` that you edit through UniversalApi's global configuration store, allowing multiple independent mappings of role IDs to gear types with optional random fallback items.【F:DiscordRoleGear/scripts/5_Mission/DiscordRoleGear.c†L6-L74】【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L3-L56】
- **Fine-grained item control** – Each `DGGearData` entry lets you define health ranges, quantity ranges, automatic attachments, and additional role requirements before the item is applied to the player’s inventory or attachment slot.【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L36-L110】
- **Structured logging** – `DGLog` writes to `$profile:/DiscordGear_<date>_<time>.log`, forwards messages to the DayZ admin log, and can forward messages into UniversalApi’s REST logging endpoint; both the on-disk and API log levels are adjustable inside the mod config.【F:DiscordRoleGear/scripts/3_Game/Logger.c†L1-L206】【F:DiscordRoleGear/scripts/5_Mission/DiscordRoleGear.c†L9-L30】

## Requirements
1. A DayZ server capable of running script mods.
2. The UniversalApi mod (load it **before** DiscordRoleGear); DiscordRoleGear declares `UniversalApi` as a required addon and will not initialize without it.【F:DiscordRoleGear/config.cpp†L3-L50】
3. A working UniversalApi backend with the Discord service configured so that `UApi().ds().GetUser` returns player role data. You will need to know the numeric Discord role IDs you want to target.

## Installation
1. Place the `@DiscordRoleGear` folder alongside your other DayZ server mods and copy its `keys` (if supplied) into the server's `keys/` directory.
2. Add UniversalApi and DiscordRoleGear to your server start parameters, for example:
   ```
   -mod=@UniversalApi;@DiscordRoleGear
   ```
3. Launch the server once so UniversalApi can create the default `DiscordRoleGear` global configuration entry (see the next section). The first boot also generates the profile log directory for the mod.【F:DiscordRoleGear/scripts/5_Mission/DiscordRoleGear.c†L12-L50】【F:DiscordRoleGear/scripts/3_Game/Logger.c†L48-L160】
4. Configure UniversalApi itself (bot token, guild ID, etc.) according to the UniversalApi documentation so that Discord lookups succeed.

## Configuration
All configuration is stored in UniversalApi’s "Globals" store under the key `DiscordRoleGear`. The mod bootstraps the data by calling `SetDefaults()` and `UApi().Rest().GlobalsLoad(...)`, then writes back with `GlobalsSave(...)` when you make changes.【F:DiscordRoleGear/scripts/5_Mission/DiscordRoleGear.c†L12-L50】 You can edit the JSON via the UniversalApi web UI or by modifying the generated file while the server is offline.

### Log levels
`LogLevel_File` and `LogLevel_API` control how noisy `DGLog` is when it writes to the profile log and UniversalApi REST logger. Raising or lowering these integers (0–3) lets you balance local disk usage and remote telemetry.【F:DiscordRoleGear/scripts/5_Mission/DiscordRoleGear.c†L9-L30】【F:DiscordRoleGear/scripts/3_Game/Logger.c†L1-L206】

### Gear sets
Each entry inside `GearSets` is a `DGGearSets` object with the following structure:

| Field | Description |
| --- | --- |
| `AttachmentSlot` | Inventory slot name (e.g., `Armband`). Leave empty to drop the item directly into the player's inventory.【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L3-L63】 |
| `RoleToGear` | Array mapping Discord role IDs to `DGGearData` definitions. The mod converts this list into a quick lookup map when it needs to equip gear.【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L9-L53】 |
| `RandomItems` | Fallback `DGGearData` entries. If none of the player’s roles match, the mod picks one of these at random.【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L11-L56】 |

A `DGGearData` object accepts these properties:

| Property | Purpose |
| --- | --- |
| `Type` | DayZ class name of the item to spawn (e.g., `Armband_Red`).【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L95-L110】 |
| `HealthPercentageLow` / `HealthPercentageHigh` | Random health range (0–100). The mod scales the item's max health by a random value within this range when it spawns.【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L68-L74】 |
| `QuanityLow` / `QuanityHigh` | Optional quantity range for stackable items (ammo, consumables, etc.).【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L74-L77】【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L95-L104】 |
| `ItemAttachments` | List of class names that should be attached to the spawned item; you can mirror the parent item’s randomized health onto these attachments by leaving `HealthToAttachments` true.【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L78-L89】【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L95-L104】 |
| `AdditionalRequiredRole` | Optional role ID that must also be present on the player, letting you gate gear behind multiple Discord roles.【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L45-L53】【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L103-L104】 |

Feel free to duplicate the provided defaults (white/yellow/orange/red armbands mapped to sample role IDs) and replace the IDs and item types with values that match your Discord guild.【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L11-L24】 Remember that role IDs are strings because Discord snowflakes exceed standard 32-bit ranges.

### Spawning multiple items
`DiscordRoleGearConfig.CreateItems(...)` loops through each configured gear set and lets them all spawn items, so you can stack several independent reward pools (e.g., armbands, starter kits, keycards).【F:DiscordRoleGear/scripts/5_Mission/DiscordRoleGear.c†L67-L74】 Simply add additional elements to `GearSets` in the JSON array.

## Awarding gear to players
`MissionServer.HandleDiscordGear(PlayerBase player)` is intentionally small so you can call it from whichever spawn hook fits your mission (e.g., after `EquipCharacter` runs or inside `InvokeOnReconnect`). The helper looks up the cached `UApiDiscordUser` for the player’s GUID and hands it to the configuration’s `CreateItems` routine.【F:DiscordRoleGear/scripts/5_Mission/MissionServer.c†L40-L47】【F:DiscordRoleGear/scripts/5_Mission/DiscordRoleGear.c†L67-L74】 Make sure you call it after the player entity exists; otherwise `parent.GetInventory()` calls will fail.

## Logging and troubleshooting
- Review `$profile:/DiscordGear_<timestamp>.log` to confirm that lookups and item creation ran; raise `LogLevel_File` if you need more detail.【F:DiscordRoleGear/scripts/3_Game/Logger.c†L48-L160】【F:DiscordRoleGear/scripts/5_Mission/DiscordRoleGear.c†L9-L30】
- UniversalApi logging can be enabled with `LogLevel_API` so you can observe Discord syncing directly from the API dashboard.【F:DiscordRoleGear/scripts/3_Game/Logger.c†L147-L206】
- If players are not receiving gear, ensure UniversalApi is returning data (watch for the "Requesting Player Discord Data" messages from `OnClientPrepareEvent`) and verify that their role IDs match the strings configured in your gear sets.【F:DiscordRoleGear/scripts/5_Mission/MissionServer.c†L24-L37】【F:DiscordRoleGear/scripts/5_Mission/RoleGear.c†L36-L56】

## Extending the mod
Because `DiscordRoleGearConfig` is a shared singleton (`GetDiscordRoleGear()`), you can read the configuration or spawn items from any other script that runs on the mission server. This makes it easy to write custom mission events or admin commands that reward players based on Discord roles without duplicating the UniversalApi plumbing.【F:DiscordRoleGear/scripts/5_Mission/DiscordRoleGear.c†L67-L85】
