def build_sum(s: str):
    numbers = [float(i) for i in s.split("	")]
    return numbers

if __name__ == '__main__':
    numbers = build_sum("24366825	33185354	21575504	88640721	97101352	27719787	71466182	49772924	64153192	44831882	48889686	70474404	49762987	51628174	81711579	36749633	59433885	55351001	34346973	44163612")
    print(sum(numbers) / len(numbers))