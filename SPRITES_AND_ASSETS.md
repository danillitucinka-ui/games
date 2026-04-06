# 🎨 Картинки для игры - Полное руководство

## 📥 Где скачать подходящие ассеты

### Бесплатные ресурсы:

#### 1. **OpenGameArt.org**
```
Поисковые запросы:
- "anime character sprite"
- "pixel art character"
- "chibi sprite sheet"
- "school background"
- "japan city background"
- "magic effects"
- "fighting game sprites"
```

#### 2. **Itch.io Free Assets**
```
Рекомендуемые ассеты:
- 2D Character Sprites (free)
- Anime Style Backgrounds
- Particle Effects Pack
- UI Elements
- Sound Effects Collection
```

#### 3. **Kenney.nl Assets**
```
Прямые ссылки:
- https://kenney.nl/assets?q=2d-characters
- https://kenney.nl/assets?q=background
- https://kenney.nl/assets?q=particles
- https://kenney.nl/assets?q=ui
```

#### 4. **OpenPixelProject**
```
Для пиксель-арта:
- Character sprites
- Environment tiles
- Effects and particles
```

## 🎯 Требуемые изображения

### Персонажи (рекомендуемый размер: 64x96px):
```
characters/
├── shirota_idle.png      # Спокойное выражение
├── shirota_attack.png    # Атакующее выражение
├── shirota_hurt.png      # Раненое выражение
├── takamine_idle.png     # Нормальное выражение
├── takamine_angry.png    # Злое выражение
├── takamine_blush.png    # С румянцем (цундере)
├── miki_happy.png        # Веселое выражение
├── seiya_serious.png     # Серьезное выражение
└── kiriko_smile.png      # Улыбающееся выражение
```

### Фоны (рекомендуемый размер: 1200x700px):
```
backgrounds/
├── school_rooftop.png    # Крыша школы с видом на город
├── classroom_day.png     # Классная комната днем
├── school_garden.png     # Школьный сад с цветами
├── city_night.png        # Ночной город с неоном
├── warehouse_dark.png    # Темный склад
└── mountain_peak.png     # Горная вершина
```

### Эффекты (разные размеры):
```
effects/
├── aura_light.png        # Светлая аура (128x128)
├── aura_dark.png         # Темная аура (128x128)
├── particles_blood.png   # Частицы крови (32x32)
├── particles_spark.png   # Искры (32x32)
├── explosion.png         # Взрыв (256x256)
├── hit_effect.png        # Эффект удара (64x64)
└── magic_circle.png      # Магический круг (128x128)
```

### UI элементы:
```
ui/
├── health_bar_bg.png     # Фон полоски здоровья
├── health_bar_fill.png   # Заполнение здоровья
├── special_meter.png     # Спецметр
├── button_normal.png     # Обычная кнопка
├── button_hover.png      # Кнопка при наведении
├── dialogue_box.png      # Окно диалога
└── menu_background.png   # Фон меню
```

## 🎵 Аудио файлы

### Звуковые эффекты (WAV, короткие):
```
sounds/
├── attack_light.wav      # Легкая атака
├── attack_heavy.wav      # Тяжелая атака
├── special_light.wav     # Светлый спецприем
├── special_dark.wav      # Темный спецприем
├── hit_normal.wav        # Обычный удар
├── hit_critical.wav      # Критический удар
├── polarity_shift.wav    # Смена полярности
├── jump.wav             # Прыжок
├── dash.wav             # Рывок
├── block.wav            # Блок
├── victory_fanfare.wav  # Победа
├── defeat_sound.wav     # Поражение
└── dialogue_blip.wav    # Звук диалога
```

### Музыка (OGG/MP3, зацикленная):
```
music/
├── menu_theme.ogg       # Тема меню
├── character_select.ogg # Выбор персонажа
├── battle_normal.ogg    # Обычный бой
├── battle_boss.ogg      # Бой с боссом
├── cutscene_emotional.ogg # Эмоциональная кат-сцена
├── victory_theme.ogg    # Тема победы
├── defeat_theme.ogg     # Тема поражения
└── credits_theme.ogg    # Титры
```

## 🛠️ Как интегрировать в игру

### 1. Структура папок:
```
ShirotaBloodPolarity/
├── assets/
│   ├── characters/
│   ├── backgrounds/
│   ├── effects/
│   ├── ui/
│   ├── sounds/
│   └── music/
└── cpp_game/
    └── src/
```

### 2. Загрузка в коде:

#### В SpriteGenerator.cpp:
```cpp
sf::Texture SpriteGenerator::loadRealSprite(const std::string& path) {
    sf::Texture texture;
    if (texture.loadFromFile(path)) {
        return texture;
    } else {
        // Fallback to procedural generation
        std::cout << "Failed to load: " << path << std::endl;
        return generateProceduralSprite("fallback");
    }
}

sf::Texture SpriteGenerator::getCharacterSprite(const std::string& name, const std::string& expression) {
    std::string path = "assets/characters/" + name + "_" + expression + ".png";
    return loadRealSprite(path);
}
```

#### В AudioManager.cpp:
```cpp
void AudioManager::loadRealAudio() {
    // Load sound effects
    loadSound("attack", "assets/sounds/attack_light.wav");
    loadSound("special", "assets/sounds/special_light.wav");
    loadSound("hit", "assets/sounds/hit_normal.wav");
    
    // Load music
    playBackgroundMusic("assets/music/menu_theme.ogg");
}
```

### 3. Обновление Game.cpp:
```cpp
// In Game constructor
spriteGenerator.loadRealSprites();
audioManager.loadRealAudio();
```

## 🎨 Рекомендации по созданию ассетов

### Стиль персонажей:
- **Аниме-style**: Большие выразительные глаза
- **Простая анимация**: 3-4 кадра на действие
- **Цветовая кодировка**: Каждый персонаж имеет уникальные цвета
- **Эмоциональные выражения**: Разные лица для разных ситуаций

### Фоны:
- **Детализированные**: Но не перегруженные деталями
- **Атмосферные**: Соответствующие настроению уровня
- **Динамичные**: С движущимися элементами (облака, листья)

### Эффекты:
- **Прозрачные**: С альфа-каналом
- **Масштабируемые**: Разные размеры для разных ситуаций
- **Цветовые**: Соответствующие полярности (синий/розовый)

## 📝 Лицензионные вопросы

### Бесплатные ассеты:
- **CC0**: Можно использовать без ограничений
- **CC-BY**: Нужно указывать автора
- **CC-BY-SA**: Требуется такое же лицензирование

### Проверка лицензий:
```json
// В credits добавить:
"Assets used:
- Character sprites: Author Name (License)
- Backgrounds: Author Name (License)
- Sound effects: Author Name (License)
- Music: Author Name (License)"
```

## 🔄 Создание собственных ассетов

Если хотите создать свои:

### Программы:
- **Aseprite**: Для пиксель-арта ($)
- **GIMP**: Бесплатный редактор изображений
- **Audacity**: Для звуков
- **LMMS**: Для музыки

### Онлайн инструменты:
- **Piskel**: Бесплатный пиксель-арт редактор
- **OpenGameArt**: Сообщество художников

## ✅ Финальный результат

С настоящими ассетами игра станет:
- **Визуально привлекательной**: Красивые спрайты вместо процедурных
- **Атмосферной**: Качественные звуки и музыка
- **Профессиональной**: Как настоящая indie игра

**Сейчас игра работает с процедурно генерируемыми спрайтами, но готова к апгрейду с реальными изображениями!** 🚀🎨