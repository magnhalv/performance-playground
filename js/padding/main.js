
function createDictEntry(value) {
  return {
  value: value,
  a1: 1, a2: 1, a3: 1, a4: 1, a5: 1, a6: 1, a7: 1, a8: 1, a9: 1, a10: 1,
  a11: 1, a12: 1, a13: 1, a14: 1, a15: 1, a16: 1, a17: 1, a18: 1, a19: 1, a20: 1,
  a21: 1, a22: 1, a23: 1, a24: 1, a25: 1, a26: 1, a27: 1, a28: 1, a29: 1, a30: 1,
  a31: 1, a32: 1, a33: 1, a34: 1, a35: 1, a36: 1, a37: 1, a38: 1, a39: 1, a40: 1,
  a41: 1, a42: 1, a43: 1, a44: 1, a45: 1, a46: 1, a47: 1, a48: 1, a49: 1, a50: 1,
  //a51: 1, a52: 1, a53: 1, a54: 1, a55: 1, a56: 1, a57: 1, a58: 1, a59: 1, a60: 1,
  //a61: 1, a62: 1, a63: 1, a64: 1, a65: 1, a66: 1, a67: 1, a68: 1, a69: 1, a70: 1,
  //a71: 1, a72: 1, a73: 1, a74: 1, a75: 1, a76: 1, a77: 1, a78: 1, a79: 1, a80: 1,
  //a81: 1, a82: 1, a83: 1, a84: 1, a85: 1, a86: 1, a87: 1, a88: 1, a89: 1, a90: 1,
  //a91: 1, a92: 1, a93: 1, a94: 1, a95: 1, a96: 1, a97: 1, a98: 1, a99: 1, a100: 1,
  //a101: 1, a102: 1, a103: 1, a104: 1, a105: 1, a106: 1, a107: 1, a108: 1, a109: 1, a110: 1,
  //a111: 1, a112: 1, a113: 1, a114: 1, a115: 1, a116: 1, a117: 1, a118: 1, a119: 1, a120: 1,
  //a121: 1, a122: 1, a123: 1, a124: 1, a125: 1, a126: 1, a127: 1, a128: 1, a129: 1, a130: 1,
  //a131: 1, a132: 1, a133: 1, a134: 1, a135: 1, a136: 1, a137: 1, a138: 1, a139: 1, a140: 1,
  //a141: 1, a142: 1, a143: 1, a144: 1, a145: 1, a146: 1, a147: 1, a148: 1, a149: 1, a150: 1,
  //a151: 1, a152: 1, a153: 1, a154: 1, a155: 1, a156: 1, a157: 1, a158: 1, a159: 1, a160: 1,
  //a161: 1, a162: 1, a163: 1, a164: 1, a165: 1, a166: 1, a167: 1, a168: 1, a169: 1, a170: 1,
  //a171: 1, a172: 1, a173: 1, a174: 1, a175: 1, a176: 1, a177: 1, a178: 1, a179: 1, a180: 1,
  //a181: 1, a182: 1, a183: 1, a184: 1, a185: 1, a186: 1, a187: 1, a188: 1, a189: 1, a190: 1,
  //a191: 1, a192: 1, a193: 1, a194: 1, a195: 1, a196: 1, a197: 1, a198: 1, a199: 1, a200: 1,
  //a201: 1, a202: 1, a203: 1, a204: 1, a205: 1, a206: 1, a207: 1, a208: 1, a209: 1, a210: 1,
  //a211: 1, a212: 1, a213: 1, a214: 1, a215: 1, a216: 1, a217: 1, a218: 1, a219: 1, a220: 1,
  //a221: 1, a222: 1, a223: 1, a224: 1, a225: 1, a226: 1, a227: 1, a228: 1, a229: 1, a230: 1,
  //a231: 1, a232: 1, a233: 1, a234: 1, a235: 1, a236: 1, a237: 1, a238: 1, a239: 1, a240: 1,
  //a241: 1, a242: 1, a243: 1, a244: 1, a245: 1, a246: 1, a247: 1, a248: 1, a249: 1, a250: 1,
  //a251: 1, a252: 1, a253: 1, a254: 1, a255: 1
  };
}

function createArrays(value, size) {
  const size_of_dict = Object.keys(createDictEntry(1)).length;
  return new Array(size).fill(new Array(size_of_dict).fill(value));
}

function createArrayOfDicts(size) {
  const dicts = [];
  for (let i = 0; i < size; i++) {
    dicts.push(createDictEntry(5));
  }
  return dicts;
}

function addDictsTogether(size, numTimes) {
  const arr_of_dicts_1 = createArrayOfDicts(size);
  const arr_of_dicts_2 = createArrayOfDicts(size);

  let result = 0;

  const start = performance.now();

  for (let i = 0; i < size; i++) {
    result += (arr_of_dicts_1[i].value + arr_of_dicts_2[i].value);
  }

  const end = performance.now();
    
  return end - start;
}

function addArraysTogether(size, numTimes) {
  const arr_of_arr_1 = createArrays(1, size);
  const arr_of_arr_2 = createArrays(2, size);

  let result = 0;

  const start = performance.now();

  for (let i = 0; i < size; i++) {
    result += (arr_of_arr_1[i][0]+ arr_of_arr_2[i][0]);
  }

  const end = performance.now();
  
  return end - start;
}

function main() {
  const N = Math.pow(2, 18);

  //console.log(`Dict: `);
  //const dict_duration = addDictsTogether(N, 30);
  //console.log(`  N = ${N}: ${dict_duration.toFixed(2) * 1000} microseconds.`);

  console.log(`Array: `);
  const arr_duration = addArraysTogether(N, 30);
  console.log(`  N = ${N}: ${arr_duration.toFixed(2) * 1000} microseconds.`);
}

main();
