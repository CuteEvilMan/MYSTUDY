# 实例：NVIDIA GeForce RTX 5060 配置参考

你的显卡信息：

- 型号：NVIDIA GeForce RTX 5060
- 驱动版本：590.48.01
- CUDA 版本：13.1

### 推荐配置步骤

1. **驱动安装**
	- 你已安装 `nvidia` 官方驱动（590.xx），无需AUR老旧包。
	- 保持 `nvidia`、`nvidia-utils`、`nvidia-dkms`（如用自编译内核）为最新。

2. **CUDA 工具链**
	- 若需开发/编译 CUDA 程序，建议安装与驱动匹配的 CUDA 13.1 工具包（可用 AUR 或官方 runfile）。
	- PyTorch/深度学习等无需单独 CUDA toolkit，只需保证驱动和 PyTorch wheel 的 CUDA 版本一致。

3. **PyTorch/深度学习环境**
	- 安装 PyTorch 时，选择 cu131（CUDA 13.1）版本 wheel。
	- 例如：
	  ```bash
	  uv pip install torch torchvision --torch-backend=cu131
	  # 或
	  uv pip install torch torchvision --index-url https://download.pytorch.org/whl/cu131
	  ```
	- 检查 torch.cuda.is_available() 是否为 True。

4. **Wayland/KDE 配置**
	- 你的桌面环境为 KDE + Wayland，务必保证已启用 DRM KMS（见上文第4节）。
	- 如遇黑屏/花屏，优先排查 KMS、驱动版本与桌面兼容性。

5. **常见问题排查**
	- 若 nvidia-smi 能正常输出，说明驱动加载无误。
	- 若 PyTorch 无法用 GPU，优先检查 CUDA 版本、PyTorch wheel 版本、驱动版本三者是否一致。

---
# Arch Linux 下 NVIDIA 驱动安装与配置

> 本文基于 Arch Wiki，适合桌面用户，涵盖显卡型号判断、驱动选择、安装、Wayland/Xorg配置、常见问题等。

## 1. 确认显卡型号

```bash
lspci -k -d ::03xx
```

根据输出在 [nouveau wiki code names](https://nouveau.freedesktop.org/CodeNames.html) 查找对应型号，确定所需驱动版本。

示例输出：

```
03:00.0 VGA compatible controller: NVIDIA Corporation GK107 [GeForce GTX 650] (rev a1)
```

GK107 属于 Kepler 架构，应选用 nvidia-470xx-dkms。

## 2. 安装驱动

### 2.1 官方支持的显卡（Turing/Ampere/Ada 及更新）

适用绝大多数 2018 年后的桌面/笔记本显卡。

```bash
sudo pacman -Syu nvidia nvidia-utils
# 如需DKMS版本（自编译内核模块）
sudo pacman -S nvidia-dkms
```


### 2.2 老旧显卡（需AUR包）
- Maxwell/Volta: `nvidia-580xx-dkms` (AUR)
- Kepler: `nvidia-470xx-dkms` (AUR)
- Fermi: `nvidia-390xx-dkms` (AUR)
- Tesla: `nvidia-340xx-dkms` (AUR)

安装AUR包示例（以yay为例）：
```bash
yay -S nvidia-470xx-dkms nvidia-470xx-utils
```
如遇依赖冲突，先卸载官方 nvidia 包。

### 2.3 32位程序支持（如需运行Steam等32位应用）

```bash
sudo pacman -S lib32-nvidia-utils
# 老卡需对应AUR包，如 lib32-nvidia-470xx-utils
```

## 3. 黑名单 nouveau

nvidia-utils 会自动黑名单 nouveau。可选：编辑 `/etc/mkinitcpio.conf`，移除 HOOKS 中的 kms，并重建initramfs：

```bash
sudo mkinitcpio -P
```

## 4. 启用 DRM KMS（Wayland/新驱动推荐）

新版驱动已默认启用。可检查：

```bash
cat /sys/module/nvidia_drm/parameters/modeset
# 输出 Y 表示已启用
```

如需手动：

```bash
echo 'options nvidia_drm modeset=1' | sudo tee /etc/modprobe.d/nvidia-drm.conf

Wayland 用户：务必保证 modeset=1，否则可能黑屏。部分桌面（如 GNOME/KDE）需重启后生效。
```

## 5. 生成 xorg 配置（如需）

通常无需手动配置。若需：

```bash
sudo nvidia-xconfig
```

或手动创建 `/etc/X11/xorg.conf.d/20-nvidia.conf`：

```
Section "Device"
	Identifier "NVIDIA Card"
	Driver "nvidia"
EndSection
```

如需指定 BusID（多卡/SLI）：
```
Section "Device"
	Identifier "NVIDIA Card"
	Driver "nvidia"
	BusID "PCI:3:0:0"
EndSection
```
```

## 6. nvidia-settings 工具

```bash
sudo pacman -S nvidia-settings
nvidia-settings # GUI配置
```

可用来调整分辨率、风扇、功耗、保存配置到 ~/.nvidia-settings-rc。
如需开机自动加载：
```bash
nvidia-settings --load-config-only
```

## 7. 多显示器/SLI/特殊配置

详见 Arch Wiki: [NVIDIA](https://wiki.archlinux.org/title/NVIDIA)

#### 多显示器
推荐用 nvidia-settings 配置，也可用 xrandr。

#### SLI/显卡直通/特殊需求
请参考 Wiki 及官方文档，部分功能需手动编辑 xorg.conf。

## 8. 常见问题

- 启动黑屏：
	- 检查 nouveau 是否被禁用（lsmod | grep nouveau 应无输出）。
	- 检查 KMS 是否配置正确。
	- 尝试加 kernel 参数：`ibt=off`（部分新CPU需）。
- Wayland 黑屏：务必先配置好 KMS，优先用 GNOME/KDE。
- Xorg 无法启动：尝试删除 ~/.nvidia-settings-rc 或 /etc/X11/xorg.conf*。
- 需显卡加速：确保已安装 nvidia-utils/lib32-nvidia-utils，glxinfo | grep NVIDIA 检查。
- 旧卡驱动需用AUR包，且需与内核版本兼容。
- Steam/游戏无3D加速：确认32位驱动已装，重启。
- 休眠/唤醒异常：尝试升级驱动，或参考 Wiki "Preserve video memory after suspend"。

## 参考
- [Arch Wiki - NVIDIA](https://wiki.archlinux.org/title/NVIDIA)
- [NVIDIA 驱动选择表](https://wiki.archlinux.org/title/NVIDIA#Supported_GPU_drivers)

- [NVIDIA/Troubleshooting](https://wiki.archlinux.org/title/NVIDIA/Troubleshooting)
- [NVIDIA/Tips and tricks](https://wiki.archlinux.org/title/NVIDIA/Tips_and_tricks)

