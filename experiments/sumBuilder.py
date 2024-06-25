def build_sum(s: str):
    numbers = [float(i) for i in s.split("	")]
    return numbers

if __name__ == '__main__':
    numbers = build_sum("790094	791038	3848534	7150238	22277780	3420676	766829	913931	1048837	651060	8533207	9807713	382950	358238	315487	630179	541469	17446171	523968	5689256")
    print(sum(numbers) / len(numbers))