# ベースイメージ
FROM ubuntu:24.04

# 非対話型で apt-get を実行するための環境変数設定
ENV DEBIAN_FRONTEND=noninteractive

# ========================================
# Timezone configuration
# ========================================
ENV TZ=Asia/Tokyo
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime \
    && echo $TZ > /etc/timezone

# 必要なパッケージのインストール
RUN apt-get update && apt-get install -y \
  # Version control
  git \
  # SSL/Certificates
  ca-certificates \
  # Compression
  zip \
  unzip \
  # Network tools
  curl \
  wget \
  net-tools \
  iputils-ping \
  # Text editors
  vim \
  nano \
  # Utilities
  less \
  jq \
  tree \
  htop \
  procps \
  # C++ toolchain
  build-essential \
  cmake \
  ninja-build \
  gdb \
  clang-format \
  clang-tidy \
  # Locale and timezone (for later steps)
  locales \
  tzdata \
  # User management (for later steps)
  sudo \
  && rm -rf /var/lib/apt/lists/*

# ========================================
# Finalize timezone setup
# ========================================
RUN dpkg-reconfigure -f noninteractive tzdata

# ========================================
# Locale Configuration
# ========================================
RUN sed -i '/ja_JP.UTF-8/s/^# //g' /etc/locale.gen \
    && locale-gen ja_JP.UTF-8 \
    && update-locale LANG=ja_JP.UTF-8

ENV LANG=ja_JP.UTF-8
ENV LANGUAGE=ja_JP:ja
ENV LC_ALL=ja_JP.UTF-8

# ========================================
# Create Non-Root User
# ========================================
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Rename existing ubuntu user to vscode if it exists, otherwise create vscode user
RUN if id -u ubuntu > /dev/null 2>&1; then \
        usermod -l $USERNAME ubuntu \
        && groupmod -n $USERNAME ubuntu \
        && usermod -d /home/$USERNAME -m $USERNAME; \
    else \
        groupadd --gid $USER_GID $USERNAME \
        && useradd --uid $USER_UID --gid $USER_GID -m -s /bin/bash $USERNAME; \
    fi \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

# ========================================
# Switch to Non-Root User
# ========================================
USER vscode
WORKDIR /home/vscode

# Suppress sudo informational message
RUN touch /home/vscode/.sudo_as_admin_successful

# ========================================
# Environment Variables
# ========================================
# CMake の既定ジェネレータを Ninja にする（-G Ninja を毎回書かなくて済む）
ENV CMAKE_GENERATOR=Ninja

# ========================================
# Working Directory
# ========================================
WORKDIR /workspaces

# ========================================
# Cleanup
# ========================================
ENV DEBIAN_FRONTEND=
