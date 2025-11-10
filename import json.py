import os

import json
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import colors
import numpy as np
import os
try:
    import matplotlib as mpl
    import imageio_ffmpeg
    mpl.rcParams['animation.ffmpeg_path'] = imageio_ffmpeg.get_ffmpeg_exe()
except Exception:
    pass

def create_combined_sorting_animation(algorithms, output_file, fps=30):
    """Create a single video that sequentially визуализирует все сортировки."""
    prepared_algorithms = []
    max_value = 0
    n_elements = None

    for algo in algorithms:
        json_file = algo['json_file']
        if not os.path.exists(json_file):
            print(f"No file: {json_file}")
            continue

        with open(json_file, 'r') as f:
            frames = json.load(f)

        if not frames:
            print(f"Empty data in: {json_file}")
            continue

        current_len = len(frames[0])
        if n_elements is None:
            n_elements = current_len
        elif current_len != n_elements:
            raise ValueError("Все алгоритмы должны иметь одинаковое количество элементов.")

        max_value = max(max_value, max(max(frame) for frame in frames))

        prepared_algorithms.append({
            **algo,
            'frames': frames
        })

    if not prepared_algorithms:
        print("Нет данных для создания общего видео.")
        return False

    fig, ax = plt.subplots(figsize=(12, 8))
    ax.set_xlabel('Индекс элемента', fontsize=12)
    ax.set_ylabel('Значение', fontsize=12)
    ax.grid(True, alpha=0.3)

    x_pos = np.arange(n_elements)
    initial_frame = prepared_algorithms[0]['frames'][0]
    bars = ax.bar(x_pos, initial_frame, color='skyblue', edgecolor='black', alpha=0.8)

    ax.set_xlim(-0.5, n_elements - 0.5)
    ax.set_ylim(0, max_value * 1.1 if max_value else 1)

    title_text = ax.set_title(prepared_algorithms[0]['title'], fontsize=16, fontweight='bold')
    info_text = ax.text(
        0.02,
        0.95,
        '',
        transform=ax.transAxes,
        fontsize=12,
        bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8)
    )

    frame_map = []
    for idx, algo in enumerate(prepared_algorithms):
        for frame_idx in range(len(algo['frames'])):
            frame_map.append((idx, frame_idx))

    total_frames = len(frame_map)

    def update(frame_num):
        algo_idx, local_idx = frame_map[frame_num]
        algo = prepared_algorithms[algo_idx]
        current_frame = algo['frames'][local_idx]

        for bar, height in zip(bars, current_frame):
            bar.set_height(height)

        title_text.set_text(algo['title'])
        info_text.set_text(
            f'Алгоритм: {algo["title"]}\nКадр: {local_idx + 1}/{len(algo["frames"])}\nЭлементов: {n_elements}'
        )

        return (*bars, info_text, title_text)

    anim = animation.FuncAnimation(
        fig,
        update,
        frames=total_frames,
        interval=1000 // fps,
        blit=True,
        repeat=False
    )

    print(f"Creating: {output_file}")
    anim.save(
        output_file,
        writer='ffmpeg',
        fps=fps,
        dpi=100,
        bitrate=2000
    )

    plt.close(fig)
    print(f"Saved: {output_file}")
    return True

def main():
    """Main function"""
    
    # make dir
    if not os.path.exists('videos'):
        os.makedirs('videos')
    
    # visual algorithms
    algorithms = [
        {
            'json_file': 'data/bubble_sort.json',
            'title': 'Bubble Sort'
        },
        {
            'json_file': 'data/selection_sort.json',
            'title': 'Selection Sort'
        },
        {
            'json_file': 'data/insertion_sort.json',
            'title': 'Insertion Sort'
        },
        {
            'json_file': 'data/shaker_sort.json',
            'title': 'Shaker Sort'
        },
        {
            'json_file': 'data/merge_sort.json',
            'title': 'Merge Sort'
        },
        {
            'json_file': 'data/heap_sort.json',
            'title': 'Heap Sort'
        }
    ]
    
    output_file = 'videos/all_sortings.mp4'
    try:
        created = create_combined_sorting_animation(
            algorithms,
            output_file,
            fps=24  # velocity of the video
        )
        if created:
            print(f"Видео сохранено: {output_file}")
        else:
            print("Не удалось создать видео.")
    except Exception as e:
        print(f"Error while creating {output_file}: {e}")

if __name__ == "__main__":
    main()