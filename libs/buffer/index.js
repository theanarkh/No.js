
const {
    console,
} = No.buildin;

function toUTF8(str) {
    const chars = [...str];
    const bytes = [];
    for (let i = 0; i < chars.length; i++) {
        const char = chars[i];
        const code = char.codePointAt(0);
        if (code > 0 && code < 0x7F) {
            bytes.push(code)
        } else if (code > 0x80 && code < 0x7FF) {
            bytes.push((code >> 6) & 0x1f | 0xC0);
            bytes.push(code & 0x3f | 0x80);  
        } else if ((code > 0x800 && code < 0xFFFF) || (code > 0xE000 && code < 0xFFFF)) {
            bytes.push((code >> 12) & 0x0f | 0xE0);
            bytes.push((code >> 6) & 0x3f | 0x80);
            bytes.push(code & 0x3f | 0x80); 
        } else if (code > 0x10000 && code < 0x10FFFF) {
            bytes.push((code >> 18) & 0x07 | 0xF0);
            bytes.push((code >> 12) & 0x3f | 0x80);
            bytes.push((code >> 6) & 0x3f | 0x80);
            bytes.push(code & 0x3f | 0x80); 
        } else if (code > 0x200000 && code < 0x3FFFFFF) {
            bytes.push((code >> 24) & 0x03 | 0xF8);
            bytes.push((code >> 18) & 0x3f | 0x80);
            bytes.push((code >> 12) & 0x3f | 0x80);
            bytes.push((code >> 6) & 0x3f | 0x80);
            bytes.push(code & 0x3f | 0x80); 
        } else {
            bytes.push((code >> 30) & 0x01 | 0xFC);
            bytes.push((code >> 24) & 0x3f | 0x80);
            bytes.push((code >> 18) & 0x3f | 0x80);
            bytes.push((code >> 12) & 0x3f | 0x80);
            bytes.push((code >> 6) & 0x3f | 0x80);
            bytes.push(code & 0x3f | 0x80); 
        }
    }
    // console.log(bytes.map((char) => { return '0x' + char.toString(16)}));
    return bytes;
}

function countByte(byte) {
    let bytelen = 0;
    while(byte & 0x80) {
        bytelen++;
        byte = (byte << 1) & 0xFF;
    }
    return bytelen || 1;
}

function fromUTF8(bytes) {
    let i = 0;
    const chars = [];
    while(i < bytes.length) {
        const byteLen = countByte(bytes[i]);
        switch(byteLen) {
            case 1:
                chars.push(String.fromCodePoint(bytes[i]));
                i += 1;
                break;
            case 2:
                chars.push(String.fromCodePoint( (bytes[i] & 0x1F) << 6 | (bytes[i + 1] & 0x3F) ));
                i += 2;
                break;
            case 3:
                chars.push(String.fromCodePoint( (bytes[i] & 0x0F) << 12 | (bytes[i + 1] & 0x3F) << 6| (bytes[i + 2] & 0x3F) ));
                i += 3;
                break;
            case 4:
                chars.push(String.fromCodePoint( (bytes[i] & 0x07) << 18 | (bytes[i + 1] & 0x3F) << 12 | (bytes[i + 2] & 0x3F) << 6 | (bytes[i + 3] & 0x3F) ));
                i += 4;
                break;
            case 5:
                chars.push(String.fromCodePoint( (bytes[i] & 0x03) << 24 | (bytes[i + 1] & 0x3F) << 18 | (bytes[i + 2] & 0x3F) << 12 | (bytes[i + 3] & 0x3F) << 6 | (bytes[i + 4] & 0x3F) ));
                i += 5;
                break;
            case 6:
                chars.push(String.fromCodePoint( (bytes[i] & 0x01) << 30 | (bytes[i + 1] & 0x3F) << 24 | (bytes[i + 2] & 0x3F) << 18 | (bytes[i + 3] & 0x3F) << 12 | (bytes[i + 4] & 0x3F) << 6 | (bytes[i + 5] & 0x3F) ));
                i += 6;
                break;
            default:
                throw new Error('invalid byte');
        }
    }
    return chars.join('');
}

class Buffer {
    bytes = null;
    memory = null;
    constructor({ length }) {
        this.memory = new ArrayBuffer(length);
        this.bytes = new Uint8Array(this.memory);
        this.byteLength = length;
    }

    toString(encoding = 'UTF-8') {
        return fromUTF8(this.bytes);
    }

    getBuffer() {
        return this.memory;
    }

    getBytes() {
        return this.bytes;
    }

    static strlen(str) {
        return toUTF8(str).length;
    }

    static from(str) {
        const chars = toUTF8(str);
        const buffer = new Buffer({length: chars.length});
        for (let i = 0; i < buffer.byteLength; i++) {
            buffer.bytes[i] = chars[i];
        }
        return buffer;
    }
    get(index) {
        return this.bytes && this.bytes[index]
    }
    set(index, value) {
        return this.bytes && (this.bytes[index] = value);
    }
    static toString(bytes) {
        return fromUTF8(bytes);
    }

    static concat(arr) {
        let len = 0;
        for (let i = 0; i < arr.length; i++) {
            len += arr[i].byteLength;
        }
        const newBuffer = Buffer.alloc(len);
        let index = 0;
        for (let i = 0; i < arr.length; i++) {
            const buffer = arr[i];
            for (let j = 0; j < buffer.byteLength; j++) {
                newBuffer.set(index++, buffer.bytes[j]);
            }
        }
        return newBuffer;
    }

    static alloc(length) {
        return new Buffer({ length });
    }
}

module.exports = Buffer;