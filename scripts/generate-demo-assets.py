#!/usr/bin/env python3
"""Generate original MIT demo artwork from geometry and deterministic random data.

No imported image pixels, logos, photographs, or external icon outlines are used.
"""

from pathlib import Path
import json
import math
import random
import struct
import zlib

ROOT = Path(__file__).resolve().parent.parent


def chunk(kind, payload):
    return struct.pack('>I', len(payload)) + kind + payload + struct.pack('>I', zlib.crc32(kind + payload))


def png_bytes(width, height, pixel):
    rows = bytearray()
    for y in range(height):
        rows.append(0)
        for x in range(width):
            rows.extend(pixel((x + 0.5) / width * 2 - 1, (y + 0.5) / height * 2 - 1))
    data = b'\x89PNG\r\n\x1a\n'
    data += chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 6, 0, 0, 0))
    data += chunk(b'tEXt', b'Copyright\x002026 David Petrie; SPDX-License-Identifier: MIT')
    data += chunk(b'tEXt', b'Source\x00scripts/generate-demo-assets.py')
    data += chunk(b'IDAT', zlib.compress(bytes(rows), 9)) + chunk(b'IEND', b'')
    return data


def png(path, width, height, pixel):
    data = png_bytes(width, height, pixel)
    path.parent.mkdir(parents=True, exist_ok=True)
    if not path.exists() or path.read_bytes() != data:
        path.write_bytes(data)


def ico(path):
    sizes = [16, 32, 48]
    images = [png_bytes(size, size, icon('icon')) for size in sizes]
    offset = 6 + 16 * len(images)
    directory = struct.pack('<HHH', 0, 1, len(images))
    for size, data in zip(sizes, images):
        directory += struct.pack('<BBBBHHII', size, size, 0, 0, 1, 32, len(data), offset)
        offset += len(data)
    path.write_bytes(directory + b''.join(images))


def segment(x, y, start, end, width=0.045):
    dx, dy = end[0] - start[0], end[1] - start[1]
    along = max(0, min(1, ((x - start[0]) * dx + (y - start[1]) * dy) / (dx * dx + dy * dy)))
    return math.hypot(x - start[0] - along * dx, y - start[1] - along * dy) <= width


def polygon(x, y, vertices):
    sides = []
    for i, a in enumerate(vertices):
        b = vertices[(i + 1) % len(vertices)]
        sides.append((b[0] - a[0]) * (y - a[1]) - (b[1] - a[1]) * (x - a[0]))
    return all(v >= 0 for v in sides) or all(v <= 0 for v in sides)


def icon(name):
    def pixel(x, y):
        radius = math.hypot(x, y)
        color = (238, 241, 244, 255)
        if name in ('target', 'reticule'):
            ink = abs(radius - 0.44) < 0.025 or (abs(x) < 0.025 and 0.24 < abs(y) < 0.76) or (abs(y) < 0.025 and 0.24 < abs(x) < 0.76)
        elif name.startswith('fa-check'):
            ink = abs(max(abs(x), abs(y)) - 0.64) < 0.04
            if name == 'fa-check':
                ink |= segment(x, y, (-0.4, 0), (-0.1, 0.32), 0.08) or segment(x, y, (-0.1, 0.32), (0.45, -0.35), 0.08)
        elif name in ('pencil', 'paint'):
            ink = polygon(x, y, [(-0.62, 0.64), (-0.4, 0.08), (0.42, -0.72), (0.72, -0.42), (-0.08, 0.4)])
            color = (255, 204, 93, 255) if name == 'pencil' else (99, 202, 219, 255)
        elif name == 'eraser':
            ink = polygon(x, y, [(-0.7, 0.15), (0.18, -0.7), (0.7, -0.15), (-0.18, 0.7)])
            color = (223, 136, 150, 255)
        elif name == 'music_player':
            ink = segment(x, y, (-0.35, -0.5), (-0.35, 0.35), 0.06) or segment(x, y, (0.35, -0.62), (0.35, 0.18), 0.06)
            ink |= segment(x, y, (-0.35, -0.5), (0.35, -0.62), 0.08)
            ink |= ((x + 0.5) / 0.22) ** 2 + ((y - 0.42) / 0.15) ** 2 <= 1
            ink |= ((x - 0.2) / 0.22) ** 2 + ((y - 0.25) / 0.15) ** 2 <= 1
        elif name in ('maps', 'skybox_viewer'):
            ink = abs(max(abs(x), abs(y)) - 0.65) < 0.04
            ink |= segment(x, y, (-0.6, 0.3), (-0.15, -0.3)) or segment(x, y, (-0.15, -0.3), (0.2, 0.25)) or segment(x, y, (0.2, 0.25), (0.62, -0.15))
        elif name == 'youtube':
            ink = polygon(x, y, [(-0.4, -0.62), (-0.4, 0.62), (0.62, 0)])
        elif name in ('vast', 'icon'):
            ink = segment(x, y, (-0.65, -0.62), (0, 0.62), 0.15) or segment(x, y, (0, 0.62), (0.65, -0.62), 0.15)
            color = (93, 204, 221, 255)
        elif 'Arrow' in name or name == 'arrow':
            ink = polygon(x, y, [(-0.58, -0.7), (0.62, 0.08), (0.1, 0.18), (-0.06, 0.72)])
        elif 'Joint' in name or name == 'jointIcon':
            ink = abs(math.hypot(x + 0.44, y) - 0.17) < 0.05 or abs(math.hypot(x - 0.44, y) - 0.17) < 0.05 or segment(x, y, (-0.25, 0), (0.25, 0), 0.04)
        else:
            count = 6 if 'Hexagon' in name else 3 if 'Triangle' in name else 4
            vertices = [(0.68 * math.cos(-math.pi / 2 + i * math.tau / count), 0.68 * math.sin(-math.pi / 2 + i * math.tau / count)) for i in range(count)]
            ink = any(segment(x, y, a, vertices[(i + 1) % count]) for i, a in enumerate(vertices))
            if 'Star' in name or 'star' in name or 'Spikes' in name:
                ink |= any(segment(x, y, (0, 0), (0.68 * math.cos(i * math.tau / 8), 0.68 * math.sin(i * math.tau / 8)), 0.025) for i in range(8))
        return color if ink else (0, 0, 0, 0)
    return pixel


def texture(name):
    random_source = random.Random('vastvr-demo-' + name)
    def pixel(x, y):
        if name.startswith('noise'):
            angle = random_source.random() * math.tau
            if 'normal' in name:
                return (round(127.5 + 127.5 * math.cos(angle)), round(127.5 + 127.5 * math.sin(angle)), 128, 255)
            level = random_source.randrange(256)
            return (level, level, level, 255)
        if name == 'wood':
            grain = 0.5 + 0.5 * math.sin(30 * x + 3 * math.sin(5 * y))
            return (round(101 + 30 * grain), round(66 + 24 * grain), round(44 + 15 * grain), 255)
        amount = max(0, min(1, (y + 1) * 0.5))
        if name == 'grey-Gradient':
            level = round(34 + 140 * amount)
            return (level, level, level, 255)
        return (round(29 + 49 * amount), round(42 + 40 * amount), round(66 + 44 * amount), 255)
    return pixel


def generate():
    output = []
    image_names = ['music_player', 'fa-check', 'fa-check-off', 'paint', 'wood', 'target', 'eraser', 'vast', 'reticule', 'pencil', 'youtube', 'skybox_viewer', 'maps']
    image_names += ['gradient' + str(i) for i in range(1, 6)] + ['grey-Gradient']
    noise_names = ['noise_normal_2', 'noise_normal_256', 'noise2d', 'noise_normal', 'noise_normal_512', 'noise', 'noise2d_64x64', 'noise_normal_256a']
    for name in image_names:
        p = ROOT / 'VastApps/resources/images' / (name + '.png')
        png(p, 256, 256, texture(name) if name.startswith('gradient') or name in ['grey-Gradient', 'wood'] else icon(name))
        output.append(str(p.relative_to(ROOT)))
    for name in noise_names:
        p = ROOT / 'VastApps/resources/noise' / (name + '.png')
        size = 64 if name.endswith('64x64') else 512 if name.endswith('512') else 256
        png(p, size, size, texture(name))
        output.append(str(p.relative_to(ROOT)))
    for name in ['wood', 'noise']:
        p = ROOT / 'VastApps/PhysicsHotel/resources' / (name + '.png')
        png(p, 256, 256, texture(name))
        output.append(str(p.relative_to(ROOT)))
    for base in ['VastDevKit/toolkit/foxtrot/Resources', 'VastDevKit/toolkit/tango/Resources', 'VastApps/VastBuilder/android', 'VastApps/4x_Resource_Game/android']:
        for p in sorted((ROOT / base).rglob('*.png')):
            data = p.read_bytes()
            width, height = struct.unpack('>II', data[16:24])
            png(p, width, height, icon('icon' if p.name.startswith('ic_launcher') else p.stem))
            output.append(str(p.relative_to(ROOT)))
    for name in ['HelloWorld.ico', 'small.ico']:
        p = ROOT / 'VastDevKit/lib/difont/examples/HelloWorld/windows/HelloWorld' / name
        ico(p)
        output.append(str(p.relative_to(ROOT)))
    manifest = ROOT / 'docs/generated-assets.json'
    manifest.write_text(json.dumps({'generator': 'scripts/generate-demo-assets.py', 'license': 'MIT', 'assets': sorted(output)}, indent=2) + '\n')
    print('Generated', len(output), 'original demo images and textures.')


if __name__ == '__main__':
    generate()
