# MYSTUDY

本项目为个人 Linux 学习与配置仓库，主要聚焦于 Arch Linux 的安装、日常使用配置、驱动与桌面环境、以及 Hyprland 等现代 Linux 桌面环境的实践与脚本工具。

## 目录结构与内容说明

```
MYSTUDY/
├── archlinux/
│   ├── archlinux-install.md      # Arch Linux 安装详细教程（精简可复制，含分区/引导/FAQ）
│   ├── archlinux-use.md          # 安装后系统配置与常用软件、桌面环境、输入法等配置说明
│   ├── nvidia.md                 # NVIDIA 显卡驱动与 CUDA、PyTorch 环境配置参考
│   ├── pytorch-uv.md             # 使用 uv 工具安装 PyTorch（多后端/多平台）详细说明
│   ├── mirrorlist                # 推荐 Arch Linux 镜像源列表
│   ├── pacman.conf               # pacman 包管理器配置示例，含 archlinuxcn 源
│   ├── Z.zshrc                   # 个人 zsh 配置，含 Oh My Zsh 插件自动安装
│   └── hyprland/
│       ├── hyprland.conf         # Hyprland 窗口管理器主配置文件
│       ├── pm                    # 亮度调节脚本（brightnessctl）
│       ├── sy                    # 音量调节脚本（wpctl）
│       └── syst                  # 系统状态监控脚本（电池/CPU/内存/磁盘/网络/温度等）
├── LICENSE
├── README.md
└── verify.txt
```

## 主要内容与亮点

- **archlinux-install.md**：
	- 详细分步讲解 Arch Linux UEFI+GPT 安装流程，含分区、挂载、基础包安装、chroot、引导配置、FAQ。
- **archlinux-use.md**：
	- 安装后网络、用户、字体、桌面环境（GNOME/KDE）、输入法、AUR、常用软件、zsh/Oh My Zsh、QQ、备份等配置。
- **nvidia.md**：
	- 针对 NVIDIA 新显卡驱动、CUDA、PyTorch 深度学习环境的配置建议。
- **pytorch-uv.md**：
	- 介绍如何用 uv 工具快速安装 PyTorch（支持 CUDA/ROCm/Intel/CPU），常见问题与 wheel 匹配说明。
- **hyprland/**：
	- Hyprland 配置文件，及自用亮度/音量/系统状态 Bash 脚本，适合 Wayland 桌面环境。
	- `pm`/`sy`/`syst` 脚本可独立调用，便于快捷键绑定。
- **Z.zshrc**：
	- 自动安装常用 zsh 插件，适合 Oh My Zsh 用户。
- **mirrorlist/pacman.conf**：
	- 推荐镜像源与 pacman 配置，含 archlinuxcn 社区源。

## 适用人群

- 有一定 Linux 基础，想快速部署 Arch Linux 桌面环境的用户
- 需要配置 NVIDIA 驱动、深度学习环境、Wayland/Hyprland 桌面的开发者
- 喜欢自动化脚本、追求高效个性化 Linux 桌面体验的进阶用户

---

如需补充 Hyprland/Sway、LUKS+LVM 加密、AUR 包安全替代等内容，欢迎 issue 或 PR。
