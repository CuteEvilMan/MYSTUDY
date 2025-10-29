# Arch Linux 安装教程（精简与可复制版）

> 本文档基于 Arch 官方安装指南并结合常见实践整理，面向已有一定 Linux 使用经验的读者。
> 阅读并理解每一步再执行。操作磁盘会破坏数据，安装前请备份重要数据。

## 目录

- 简介与前置条件
- 制作启动 U 盘（dd / Ventoy）
- 从 U 盘 启动并联网
- 磁盘分区与文件系统（UEFI+GPT 示例）
- 挂载、选择镜像并安装基础系统
- 生成 fstab 与进入 chroot
- 系统配置（时区、locale、主机名、用户）
- 安装引导程序（GRUB）
- 启用常用服务与收尾
- 常见问题与参考链接

## 简介与前置条件

- 目标：在一台 UEFI 机器上安装 Arch Linux（GPT 分区）。
- 要求：能访问互联网，能使用另一个 Linux 系统制作启动盘或在 Windows 下制作。备份好数据。
- 目标：在一台 UEFI 机器上安装 Arch Linux（GPT 分区）。
- 要求：能访问互联网，能使用另一个 Linux 系统制作启动盘或在 Windows 下制作。备份好数据。
- 推荐在安装前查看官方安装文档：[Arch 安装指南](https://wiki.archlinux.org/title/Installation_guide)

## 制作启动 U 盘

方法一：使用 dd（简单、原始）

```bash
sudo dd if=/path/to/archlinux-*.iso of=/dev/sdX bs=4M status=progress && sync
```

将 `/dev/sdX` 替换为 U 盘设备节点（例如 `/dev/sdb`）。千万不要写到分区（如 `/dev/sdb1`）或系统盘。

方法二：Ventoy（支持多 iso）

- 下载并解压 Ventoy，进入解压目录后运行：

```bash
sudo sh Ventoy2Disk.sh -i /dev/sdX
```

- 常用选项：`-I` 强制安装，`-s` 启用安全启动，`-g` 使用 GPT。
- 注意：务必确认设备名，错误会格式化系统磁盘。

## 从 U 盘 启动并联网

1. 从 U 盘启动，选择 Arch ISO 进入 live 环境。
1. （可选）确认网络：

有线网络通常自动连通。无线使用 `iwctl`：

```text
iwctl
iwd> device list
iwd> station wlan0 scan
iwd> station wlan0 get-networks
iwd> station wlan0 connect <SSID>
```

1. 可以用 `ping -c3 archlinux.org` 检查连通性。

## 磁盘分区（UEFI + GPT 示例）

这是一个常见的分区方案（根据磁盘大小调整）：

- /dev/sda1 — EFI System Partition — 512M — type EF00
- /dev/sda2 — swap — e.g. 4–16G — type 8200
- /dev/sda3 — / (根) — e.g. 30G — type 8300
- /dev/sda4 — /home — 剩余空间 — type 8300

用 `gdisk`（或 `parted`、`cfdisk`）进行分区：

```bash
gdisk /dev/sda
# 在 gdisk 中创建分区：新建、写入
```

示例：创建完分区后格式化并启用 swap：

```bash
mkfs.fat -F32 /dev/sda1           # EFI 分区
mkfs.ext4 /dev/sda3               # 根分区
mkfs.ext4 /dev/sda4               # /home
mkswap /dev/sda2
swapon /dev/sda2
```

挂载：

```bash
mount /dev/sda3 /mnt --mkdir
mount /dev/sda4 /mnt/home --mkdir
mount /dev/sda1 /mnt/boot --mkdir
```

如果使用 LUKS/LVM，这里会多出加密与卷组步骤（本文档不包含加密示例）。

## 选择镜像并更新 pacman 数据库

推荐在安装前选择速度较快的镜像。示例命令：

```bash
curl 'https://archlinux.org/mirrorlist/?country=CN&protocol=http&protocol=https&ip_version=4&use_mirror_status=on' -o /etc/pacman.d/mirrorlist.tmp
sed -i 's/^#Server/Server/' /etc/pacman.d/mirrorlist.tmp
# 可选: 使用 rankmirrors 对镜像排序（需要安装 pacman-contrib）
rankmirrors /etc/pacman.d/mirrorlist.tmp > /etc/pacman.d/mirrorlist
pacman -Syy
```

注意：在 live 环境中修改 `/etc/pacman.d/mirrorlist` 需有写权限；也可以先保存到 `/mnt/etc/pacman.d/mirrorlist` 供 chroot 后使用。

## 安装基础系统

使用 `pacstrap` 安装基础包：

```bash
pacstrap -K /mnt base linux linux-firmware --needed
```

根据需要添加内核微码与常用软件：

```bash
pacstrap /mnt base linux linux-firmware intel-ucode amd-ucode --needed
```

## 生成 fstab 并 chroot

```bash
genfstab -U /mnt >> /mnt/etc/fstab
cat /mnt/etc/fstab
arch-chroot /mnt
```

## 在 chroot 中的系统配置

安装并配置常用软件包：

```bash
pacman -Syu --needed vim sudo networkmanager git base-devel e2fsprogs sof-firmware curl wget
```

设置时区：

```bash
ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
hwclock --systohc
```

配置语言环境（示例启用中英文 UTF-8）：

编辑 `/etc/locale.gen`，取消注释：

```text
en_US.UTF-8 UTF-8
zh_CN.UTF-8 UTF-8
```

然后运行：

```bash
locale-gen
echo 'LANG=en_US.UTF-8' > /etc/locale.conf
```

设置主机名：

```bash
echo 'myhostname' > /etc/hostname
```

设置 root 密码：

```bash
passwd
```

创建普通用户并加入 wheel 组（以便使用 sudo）：

```bash
useradd -m -G wheel -s /bin/bash youruser
passwd youruser
pacman -S sudo
EDITOR=vim visudo # 取消 %wheel 的注释以允许 wheel 组使用 sudo
```

启用网络管理（建议启用 NetworkManager）：

```bash
systemctl enable --now NetworkManager
```

## 安装引导加载程序（GRUB，UEFI）

安装 grub 与 efibootmgr：

```bash
pacman -S --needed grub efibootmgr
grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=GRUB
grub-mkconfig -o /boot/grub/grub.cfg
```

如果需要多系统，还需要安装 os-prober

如果使用了 `intel-ucode` 或 `amd-ucode`，确保 grub 配置中包含微码（默认 pacman 脚本通常会处理）。

## 收尾与重启

退出 chroot 并卸载：

```bash
exit
umount -R /mnt
swapoff -a
reboot
```

拔掉安装 U 盘，系统应从硬盘启动进入新安装的 Arch。

## 常见问题（FAQ）

- 无法联网：检查网卡驱动是否加载，查看 `ip link`、`dmesg`。无线用 `iwctl` 或 `iw`/`wpa_supplicant`。
- 找不到 EFI 分区：确认使用 UEFI 启动且已创建 FAT32 的 EFI 分区并挂载到 `/boot`（或 `/boot/efi`，视习惯）。
- grub 安装失败：检查 `--efi-directory` 指向已挂载且为 FAT32 的 EFI 分区，且已安装 `efibootmgr`。

## 参考与延伸阅读

- Arch Wiki — Installation guide: [https://wiki.archlinux.org/title/Installation_guide](https://wiki.archlinux.org/title/Installation_guide)
- Arch Wiki — GRUB: [https://wiki.archlinux.org/title/GRUB](https://wiki.archlinux.org/title/GRUB)
- Ventoy: [https://www.ventoy.net](https://www.ventoy.net)

