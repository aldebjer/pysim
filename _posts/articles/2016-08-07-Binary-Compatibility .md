---
layout: post
title: Binary compatibility
excerpt: "Binary compatibility means that if you compile a c++ system for a version of pysim it will still work for future relases..."
modified: 2016-08-07T22:08:00-04:00
categories: blog
tags: [development]
comments: false
share: true
---

# Binary compatibility
Binary compatibility for pysim basically means that if you compile a c++ system for a version of pysim it will still work for future relases of PySim. This if obviously important since it saves everybody the trouble of recompiling as soon as something changes in a minor version of PySim. 

### Implementation
Some refactoring has been made to hide some of the implementation from the child classes of Systems, i.e. all third party system implementations. This is acheived by using [d-pointers](https://wiki.qt.io/D-Pointer), a concept borrowed from trolltech and Qt. The pointers contains most of the data of each system, and is only visible to the implementation of CommonSystem, and is not visible to the child systems. This means that the interface to the child systems is less likely to change when the implementation of PySim changes, and that there is no need to re compile them. For a detailed instruction of Binary compatability see for example the [KDE policies](https://community.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B) on the matter.

### Results
One immediate result is that the systems will be faster to compile, since they are less complex - more implementation is hidden in the CommonSystem implementation. Over time the most important result is that the systems will not need to be recompiled as often. There might still be changes to the interface exposed, but these changes will be a lot less common. The refactoring resulted in a lot less code duplication as well, which is always nice. 
