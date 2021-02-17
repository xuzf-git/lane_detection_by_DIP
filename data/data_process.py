import os
import json

# 读入 groundtruth
gt_all = {}
gt = []
with open("./groundtruth.json", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        line_dic = json.loads(line)
        gt_all[line_dic["raw_file"]] = line

# 遍历图片
find_img = os.walk(r"selected test data")


# 创建文件夹保存测试图片
count = 0

for path, dir_list, file_list in find_img:
    for file_name in file_list:
        if file_name == "20.jpg":
            count += 1
            name = os.path.join(path, file_name)
            name = name.replace('\\', '/');
            name = name.replace("./selected test data", "clips")
            gt.append(gt_all[name])
            print(name)
        else:
            os.remove(os.path.join(path, file_name))

print("total image num: ", count)

# 存储 test 图像的 groundtruth
with open("groundtruth.json", 'w', encoding='utf8') as f:
    for sample in gt:
        f.write(sample)

