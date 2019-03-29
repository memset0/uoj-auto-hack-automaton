# UOJ Auto Hack Automaton

爬取 UOJ 的提交记录并测试给定的数据能否成功 Hack

#### 使用方法

首先需要安装依赖 `Python3` & `requests` & `pyyaml` 。

把输入文件放入 `data.in` ，答案文件放入 `data.ans` ，修改 `config.yml` 里的 `problem` 为你需要 auto-hack 的题目编号。

运行 `python3 main.py` 即可。

#### TODO

* 放入 data maker 自动生成数据对拍

* 接入 ROJ / SYZOJ 的 judge 端
