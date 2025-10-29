现在讲解安装完archlinux后如何配置使得我们能正常使用
主要是我自己的配置

```markdown
# Arch Linux — 使用配置速成（个人配置示例）

下面是一个可复制的、结构化的配置说明，基于你已有的安装环境（TTY / chroot 已完成并能联网）。文档以可执行命令为主，必要处给出说明和替代项。

> 注意：以下命令会更改系统配置与安装软件。执行前请确认目标系统与分区无误并有网络连接。

## 目录

- 启用并配置网络
- 添加用户与 sudo
---
# Arch Linux — 使用配置速成（个人配置示例）

下面是一个可复制的、结构化的配置说明，基于你已有的安装环境（TTY / chroot 已完成并能联网）。文档以可执行命令为主，必要处给出说明和替代项。

> 注意：以下命令会更改系统配置与安装软件。执行前请确认目标系统与分区无误并有网络连接。

## 目录

- 启用并配置网络
- 添加用户与 sudo
- 安装字体
- 安装桌面环境（GNOME / KDE 简要）
- 安装中文输入法（fcitx5）
- 添加 `archlinuxcn` 源与密钥
- 常用软件（AUR、浏览器、办公、蓝牙、节能）
- zsh 与 Oh My Zsh
- LinuxQQ（适配 fcitx5）
- 其它说明与仓库

## 启用并配置网络

切换到你的 TTY（或在 chroot 外的已安装系统中）并启用 NetworkManager：

```bash
sudo systemctl enable --now NetworkManager
```

使用 nmcli 连接无线网络：

```bash
nmcli device wifi list                       # 列出附近的 WiFi 热点
sudo nmcli device wifi connect <SSID> password "<密码>"  # 连接
```

如果你喜欢交互式连接，可以省略密码并加 `--ask` 参数来提示输入。

## 添加用户与 sudo

创建普通用户并设置密码：

```bash
sudo useradd -m -s /bin/bash NB
sudo passwd NB
```

给用户 sudo 权限（通过 wheel 组）：

```bash
sudo pacman -S --needed sudo
sudo usermod -aG wheel NB
sudo EDITOR=vim visudo   # 取消 %wheel 行的注释以允许 wheel 使用 sudo
```

如果系统提示缺少 `vi`，可以用 `vim` 代替或建立软链接：

```bash
sudo ln -s /usr/bin/vim /usr/bin/vi
```

## 安装字体

常用字体安装示例（按需选择）：

```bash
sudo pacman -S --needed noto-fonts noto-fonts-cjk noto-fonts-extra ttf-dejavu ttf-roboto ttf-liberation
# 可选：额外中文字体
sudo pacman -S --needed wqy-microhei wqy-bitmapfont adobe-source-han-sans-cn-fonts adobe-source-han-serif-cn-fonts
```

如果你需要 Nerd Font（用于图标/状态栏），可以安装对应包或从 AUR 获取。

## 安装桌面环境（简要）

GNOME（Wayland）：

```bash
sudo pacman -S --needed gdm gnome gnome-extra
sudo systemctl enable --now gdm
```

KDE Plasma（示例）：

```bash
sudo pacman -S --needed plasma-meta kde-applications-meta sddm
sudo systemctl enable --now sddm
```

根据需要选择安装 Wayland 或 X11 相关包。

## 安装中文输入法（fcitx5）

```bash
sudo pacman -S --needed fcitx5 fcitx5-configtool fcitx5-gtk fcitx5-qt
```

安装后可用 `fcitx5-configtool` 进行图形化配置，或者编辑 `~/.config/fcitx5/config`。

启动并设置环境变量（针对图形桌面会话）：

```bash
# 在 ~/.xprofile 或 ~/.pam_environment 中加入
export GTK_IM_MODULE=fcitx
export QT_IM_MODULE=fcitx
export XMODIFIERS=@im=fcitx
```

## 添加 `archlinuxcn` 源与密钥

编辑 `/etc/pacman.conf`（需要 root）：

```ini
[archlinuxcn]
Server = https://mirrors.cernet.edu.cn/archlinuxcn/$arch
Server = https://mirrors.tuna.tsinghua.edu.cn/archlinuxcn/$arch
Server = https://mirrors.hust.edu.cn/archlinuxcn/$arch
```

然后导入并签署 keyring：

```bash
sudo pacman -S --needed archlinuxcn-keyring
# 如果遇到签名警告，可手动签署（示例）
sudo pacman-key --lsign-key "farseerfc@archlinux.org"
sudo pacman -Syyu
```

## 常用软件（AUR、浏览器、办公、蓝牙、节能）

安装 AUR 助手（例如 paru）：

```bash
sudo pacman -S --needed paru
# 或者安装 yay（任选其一）
```

浏览器示例：

```bash
sudo pacman -S --needed chromium firefox
# 如果使用 GNOME，可安装 gnome-browser-connector
sudo pacman -S --needed gnome-browser-connector
```

办公软件：

```bash
sudo pacman -S --needed libreoffice libreoffice-fresh-zh-cn
```

蓝牙：

```bash
sudo pacman -S --needed bluez bluez-utils
sudo systemctl enable --now bluetooth.service
```

节能与电源管理（笔记本推荐）：

```bash
sudo pacman -S --needed tlp powertop
sudo systemctl enable --now tlp
```

开启防火墙（示例）：

```bash
sudo pacman -S --needed ufw
sudo systemctl enable --now ufw
sudo ufw enable
```

## zsh 与 Oh My Zsh

安装 zsh 并切换默认 shell（按需）：

```bash
sudo pacman -S --needed zsh curl
chsh -s /bin/zsh NB   # 将 NB 替换为你的用户名
```

安装 Oh My Zsh：

```bash
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
# 如果你有自定义的配置文件，可以复制：
cp Z.zshrc ~/.zshrc
source ~/.zshrc
```

## LinuxQQ（在 Wayland + GNOME 下适配 fcitx5）

如果使用 AUR 的 `linuxqq` 脚本，可按下列方法确保输入法可用：

```bash
paru -S --needed linuxqq
# linuxqq 通常会在 /usr/bin/linuxqq 下有脚本
which linuxqq
sudo vim /usr/bin/linuxqq
# 将启动命令修改为：
# Exec=env GTK_IM_MODULE=fcitx QT_IM_MODULE=fcitx XMODIFIERS=@im=fcitx /opt/QQ/qq
```

也可拷贝并修改 desktop 文件以避免改动系统文件：

```bash
cp /usr/share/applications/qq.desktop ~/.local/share/applications/
vim ~/.local/share/applications/qq.desktop
# 示例 Exec 行：
# Exec=env GTK_IM_MODULE=fcitx QT_IM_MODULE=fcitx XMODIFIERS=@im=fcitx linuxqq %U
# 如果你希望在终端中运行以便观察输出，可将 Terminal=true
```

## 其它说明与仓库

如果你想备份或同步我的配置仓库：

```bash
git clone https://github.com/CuteEvilMan/MYSTUDY.git
```

---

以上为一个可直接使用的个人配置示例。我已修正原文中若干拼写（例如 NetworkManager、systemctl）并统一了代码块与说明。如果你要我：

- 添加 Hyprland / Wayland 特定配置示例（比如 sway/Hyprland 配置）、
- 补充 LUKS + LVM、或
- 将部分 AUR 包换成安全替代（避免未签名包），

请告诉我优先项，我会继续完善并把 `todo` 状态更新为完成。
