f = open('zoo.vd', 'r')
fw = open('zoo_washed.vd', 'w')

for line in f.readlines():
    haha = line.split()
    print(haha)

    for i in range(len(haha)):
        fw.write(haha[i])
        if i == len(haha) - 1:
            fw.write('\n')
        else:
            fw.write('\t')