# ⚠️ 注意事项与实用提醒

- PyTorch GPU 版本需本机已正确安装对应版本的 NVIDIA 驱动和 CUDA（如 nvidia-smi 能正常输出信息）。
- CUDA 版本需与 PyTorch wheel 匹配（如 cu128=CUDA 12.8，cu121=CUDA 12.1），否则会报错或无法调用 GPU。
- 若只需 CPU 推理/训练，建议直接用 CPU-only 版本，兼容性最好。
- MacOS 仅支持 CPU 版，Linux/Windows 才有 CUDA/ROCm/Intel GPU 支持。
- 使用 uv pip --torch-backend=auto 可自动检测本机环境，推荐新手优先尝试。
- 若遇 GPU 不可用、torch.cuda.is_available() 为 False，优先检查驱动、CUDA 版本和 PyTorch 安装源是否一致。

---
# 使用 uv 安装 PyTorch 教程（多后端/多平台）

> 适用 uv >= 0.5.3，支持 CPU/CUDA/ROCm/Intel GPU，推荐优先阅读本教程再动手。

## 1. 基本原理与注意事项

- PyTorch 官方大部分 wheel 不在 PyPI，而在 https://download.pytorch.org/whl/ 下的专用 index。
- 不同加速后端（CPU/CUDA/ROCm/Intel）需用不同 index，且包名带有 local version（如 2.5.1+cu128）。
- uv 支持通过 pyproject.toml 配置 index，也支持命令行一键自动选择后端。

## 2. 新建项目并初始化 uv

```bash
uv init --python 3.10  # 选择你的Python版本
uv add torch torchvision  # 默认从PyPI安装（Linux下为CUDA 12.8，Win/mac为CPU）
```

此时 pyproject.toml 依赖如下：

```toml
[project]
name = "my-pytorch-project"
version = "0.1.0"
requires-python = ">=3.10"
dependencies = [
	"torch>=2.9.1",
	"torchvision>=0.24.1",
]
```

## 3. 配置不同后端的 PyTorch 源

### 3.1 全平台 CPU-only

```toml
[tool.uv.sources]
torch = [ { index = "pytorch-cpu" } ]
torchvision = [ { index = "pytorch-cpu" } ]

[[tool.uv.index]]
name = "pytorch-cpu"
url = "https://download.pytorch.org/whl/cpu"
explicit = true
```

### 3.2 Linux/Win 用 CUDA，其他平台用 CPU

以 CUDA 12.8 为例：

```toml
[tool.uv.sources]
torch = [
	{ index = "pytorch-cpu", marker = "sys_platform != 'linux'" },
	{ index = "pytorch-cu128", marker = "sys_platform == 'linux'" },
]
torchvision = [
	{ index = "pytorch-cpu", marker = "sys_platform != 'linux'" },
	{ index = "pytorch-cu128", marker = "sys_platform == 'linux'" },
]

[[tool.uv.index]]
name = "pytorch-cpu"
url = "https://download.pytorch.org/whl/cpu"
explicit = true

[[tool.uv.index]]
name = "pytorch-cu128"
url = "https://download.pytorch.org/whl/cu128"
explicit = true
```

### 3.3 ROCm/Intel GPU

ROCm（AMD GPU）：
```toml
[tool.uv.sources]
torch = [ { index = "pytorch-rocm", marker = "sys_platform == 'linux'" } ]
torchvision = [ { index = "pytorch-rocm", marker = "sys_platform == 'linux'" } ]
pytorch-triton-rocm = [ { index = "pytorch-rocm", marker = "sys_platform == 'linux'" } ]

[[tool.uv.index]]
name = "pytorch-rocm"
url = "https://download.pytorch.org/whl/rocm6.4"
explicit = true
```

Intel GPU：
```toml
[tool.uv.sources]
torch = [ { index = "pytorch-xpu", marker = "sys_platform == 'win32' or sys_platform == 'linux'" } ]
torchvision = [ { index = "pytorch-xpu", marker = "sys_platform == 'win32' or sys_platform == 'linux'" } ]
pytorch-triton-xpu = [ { index = "pytorch-xpu", marker = "sys_platform == 'win32' or sys_platform == 'linux'" } ]

[[tool.uv.index]]
name = "pytorch-xpu"
url = "https://download.pytorch.org/whl/xpu"
explicit = true
```

### 3.4 可选依赖切换后端（如 uv sync --extra cu128）

```toml
[project.optional-dependencies]
cpu = [ "torch>=2.9.1", "torchvision>=0.24.1" ]
cu128 = [ "torch>=2.9.1", "torchvision>=0.24.1" ]

[tool.uv]
conflicts = [ [ { extra = "cpu" }, { extra = "cu128" } ] ]

[tool.uv.sources]
torch = [
	{ index = "pytorch-cpu", extra = "cpu" },
	{ index = "pytorch-cu128", extra = "cu128" },
]
torchvision = [
	{ index = "pytorch-cpu", extra = "cpu" },
	{ index = "pytorch-cu128", extra = "cu128" },
]

[[tool.uv.index]]
name = "pytorch-cpu"
url = "https://download.pytorch.org/whl/cpu"
explicit = true

[[tool.uv.index]]
name = "pytorch-cu128"
url = "https://download.pytorch.org/whl/cu128"
explicit = true
```

## 4. 命令行直接安装 PyTorch

如不想写 pyproject.toml，可直接用 uv pip：

CPU-only：
```bash
uv pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cpu
```

CUDA 12.8：
```bash
uv pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu128
```

## 5. 自动后端选择（推荐）

uv 支持自动检测本机 GPU 并选择最优后端：

```bash
# 自动选择（推荐）
uv pip install torch --torch-backend=auto
# 或指定后端
uv pip install torch torchvision --torch-backend=cu128
# 环境变量方式
UV_TORCH_BACKEND=cu128 uv pip install torch torchvision
```

## 6. 常见问题

- CUDA 版本不兼容：确保本机驱动 >= 对应 CUDA 版本。
- MacOS 不支持 CUDA/ROCm，仅能用 CPU 版。
- 安装慢/失败：可尝试更换 PyTorch 官方 index，或用国内镜像（如清华源同步部分 wheel）。
- uv 版本过低：升级 uv 至最新版。

## 7. 参考

- [PyTorch 官网安装说明](https://pytorch.org/get-started/locally/)
- [uv 官方文档](https://docs.astral.sh/uv/guides/pytorch/)
- [Arch Wiki - PyTorch](https://wiki.archlinux.org/title/PyTorch)
