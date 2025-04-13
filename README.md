# MarvelRivals DMA

A **simple, high performance Marvel Rivals DMA cheat** utilizing **Dear ImGUI** for the overlay/menu and **DMALibrary** for memory reading. It communicates with a **Kmbox NET/BPro/MAKCU** for aimbot. This project demonstrates how **scatter reads and caching** can significantly boost speed when using DMA.

> ⚠ **Disclaimer:** This project is for **educational purposes only**. It cannot be used out of the box, as all offsets have been stripped. Do not use this to cheat.

## 🚀 Key Optimizations

- **Scatter reading** for almost everything, except for the initial **Player Array** and **Player States**.
- **Smart caching**: Only reads and caches **UWorld**, **Game Instance**, and other rarely changing pointers when necessary.
- **Reduced memory reads**: Slower-updating pointers (e.g., **Player Array & Player States**) are only refreshed **every 5 seconds**.
- **Batch reads for efficiency**: Critical data such as **camera and bone values** are batched into a **single scatter read**.
- **Manual memory refreshing** using `-norefresh`.
- **Optimized player reads**: Entire **Player Array** and **Bone Array** are read in one go instead of scatter reading each pointer in the array, reducing overhead and improving performance.
- **Multithreaded rendering** for better performance.

## 📋 Potential Improvements

- **Pre-emptive reading**: Scatter reading the **Player State array** alongside the **Player Array** would mean that if the Player Array hasn't changed the Player State array would already be obtained.
- **Dynamic bones**: Automatically retrieving the **head bone** for different characters instead of hardcoding values.

## 🛠️ Notes

- **Multithreading for organization**: The separate threads used for reading **are not for performance gains**—DMA reads are the bottleneck. Instead, multithreading is used for **better cache management** and code organization.

## Credits:

[ImGUI](https://github.com/ocornut/imgui)

[DMALibrary](https://github.com/Metick/DMALibrary)

[MemProcFS](https://github.com/ufrisk/MemProcFS)
