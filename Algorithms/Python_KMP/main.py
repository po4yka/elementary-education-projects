def kmp(P, T):
    res = []

    K = []
    t = -1
    K.append(t)

    for k in range(1, len(P) + 1):
        while t >= 0 and P[t] != P[k - 1]:
            t = K[t]
        t = t + 1
        K.append(t)
    # print(K)

    m = 0
    for i in range(0, len(T)):
        while m >= 0 and P[m] != T[i]:
            m = K[m]
        m = m + 1
        if m == len(P):
            # print(i - m + 1, i)
            res.append(i - m + 1)
            m = K[m]

    if not res:
        print(-1)
    else:
        print(*res, sep=",")

def main():
    template = input()
    text = input()
    kmp(template, text)


if __name__ == "__main__":
    main()
