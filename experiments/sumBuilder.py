def build_sum(s: str):
    numbers = [float(i) for i in s.split("	")]
    return numbers

if __name__ == '__main__':
    numbers = build_sum("256922	253526	242855	264006	268044	244629	261917	246681	274763	245294	253598	264592	259035	254960	278985	260223	270869	261764	264138	248109")
    print(sum(numbers) / len(numbers))