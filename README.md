# 🔥 KeccakCube: The Recursive Entropy Engine

Welcome to the blacksite.

### 📄 **Dual License Strategy**
```
KeccakCube is available under a dual licensing model:

1. RESEARCH & ACADEMIC USE:
   GNU Affero General Public License v3.0 (AGPL-3.0)
   https://www.gnu.org/licenses/agpl-3.0.html
   
   - Free for research, academic, and non-commercial use
   - Any modifications must be shared under same license
   - Source code must be made available if distributed
   - Perfect for academic papers, research collaborations

2. COMMERCIAL USE:
   Proprietary Commercial License
   
   - Contact: michael.doran.808@gmail.com for commercial licensing terms
   - Allows proprietary modifications and closed-source distribution
   - Removes copyleft requirements for commercial applications
   - Supports enterprise deployment without source disclosure

Choose the license that fits your use case.
```

This is **KeccakCube**, a post-quantum, entropy-hardened hashing engine built from the bones of Keccak, fractal logic, and 3D rotational chaos. It shreds reality into 4095 rounds of non-linear cube transformations across 32 threads of hell.

It **passes Dieharder**.  
It **maxes ENT**.  


## ⚙️ Features

- 🧠 **4095-round transformation** pipeline: `theta → rho → pi → chi → iota`
- 🧊 **16x16x16 3D cube** state per thread
- 🔁 **32-thread parallel cube hashing**
- 🔐 Entropy harvesting from `/dev/urandom`, real-time clocks, and system DMI paths
- 🌀 Custom **rotation offset matrix**
- 🔂 **LFSR-derived round constants**
- 📦 Final output: **144-byte squeezed hash** from merged cube state

## 🚀 Usage

```bash
make build
./keccak_cube
```
Outputs a 144-byte hex-encoded final hash squeezed from chaotic thread-woven entropy cubes.

🧪 Entropy Validation
	•	✔️ Dieharder battery: Passes all tests with perfect uniformity
	•	✔️ ENT test suite: Maxes out entropy, chi-square, and serial correlation thresholds
	•	✔️ Formal Proofs: Authored and maintained privately by Michael A. Doran Jr.

This is not a toy. This is entropy architecture.

🧬 Applications
	•	Recursive entropy stretchers
	•	Post-quantum hashing research
	•	Symbolic cryptographic art
	•	Multi-threaded hash pipelines
	•	High-entropy secure seed generators

📦 Build

Follows a clean two-phase Docker and Makefile structure:

all: build

build: ;@gcc -O3 -o keccak_cube_nuke keccak_cube_nuke.c -lpthread
test: ;@./keccak_cube_nuke
clean: ;@rm -f keccak_cube_nuke

📜 License

MIT License

Copyright (c) 2025 Michael A. Doran Jr.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”),
to deal in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
express or implied, including but not limited to the warranties of
merchantability, fitness for a particular purpose and noninfringement.
In no event shall the authors or copyright holders be liable for any claim,
damages or other liability, whether in an action of contract, tort or otherwise,
arising from, out of or in connection with the Software or the use
or other dealings in the Software.

⸻

This engine is dedicated to all those who resist entropy collapse.
If you’re reading this, the mirror broke before they could finish the loop.

—
Michael A. Doran Jr.
patreon.com/michaeldoran
