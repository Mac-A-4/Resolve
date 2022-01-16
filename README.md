# Resolve

An external implementation of an address resolver for Windows. You can resolve addresses in external processes with a simple expression in a string.

# Features

- Resolve addresses of exports inside modules
- Apply decimal/hexadecimal offsets

# Example

![Example Output](/example.png?raw=true)

# Syntax

## General

An expression string is made up of an arbitrary number of tokens, separated by either +/- signs.

## Reference Tokens

A reference token is a token within the expression that references an export in a module in the target process.

Some examples are:

```
<module_name.dll>:export_name
<kernel32.dll>:HeapAlloc
<ntdll.dll>:LdrLoadDll
```

## Decimal Offset Tokens

Placing a decimal offset in the expression is as simple as typing a number in decimal format. Do not add any leading zeroes, because a leading zero pushes the parser to parse the constant as hexadecimal.

Here's an example:

```
<module_name.dll>:export_name + 20
<kernel32.dll>:HeapAlloc + 45
<ntdll.dll>:LdrLoadDll - 21
```

## Hexadecimal Offset Tokens

Hexadecimal offsets are handled the same way as decimal offsets, except a leading zero is added to the constant to indicate a hexadecimal value.

This system is similar to how octal constants are written out in C, there is no x following the zero.

Here's an example:

```
<module_name.dll>:export_name + 020
<kernel32.dll>:HeapAlloc + 0DEADBEEF
<ntdll.dll>:LdrLoadDll - 020203F
```