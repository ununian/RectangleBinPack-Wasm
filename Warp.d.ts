// define type aliases for various native number types
type Char = number;
type SignedChar = number;
type UnsignedChar = number;
type Short = number;
type UnsignedShort = number;
type Int = number;
type UnsignedInt = number;
type Long = number;
type UnsignedLong = number;
type Float = number;
type Double = number;

export enum MaxRectsBinPack_FreeRectChoiceHeuristic {
	RectBestShortSideFit = 0,
	RectBestLongSideFit = 1,
	RectBestAreaFit = 2,
	RectBottomLeftRule = 3,
	RectContactPointRule = 4,
}

export class VectorRect {
constructor();
	
	push_back(arg0: Rect): void;
	
	resize(arg0: UnsignedLong, arg1: Rect): void;
	
	size(): UnsignedLong;
	
	get(arg0: UnsignedLong): any;
	
	set(arg0: UnsignedLong, arg1: Rect): boolean;
	delete(): void;
}

export class Rect {
constructor();
constructor(arg0: Int, arg1: Int, arg2: Int, arg3: Int);
	x: Int
	y: Int
	width: Int
	height: Int
	delete(): void;
}

export class VectorRectSize {
constructor();
	
	push_back(arg0: RectSize): void;
	
	resize(arg0: UnsignedLong, arg1: RectSize): void;
	
	size(): UnsignedLong;
	
	get(arg0: UnsignedLong): any;
	
	set(arg0: UnsignedLong, arg1: RectSize): boolean;
	delete(): void;
}

export class RectSize {
constructor();
constructor(arg0: Int, arg1: Int);
	width: Int
	height: Int
	delete(): void;
}

export class MaxRectsBinPack {
constructor();
constructor(arg0: Int, arg1: Int, arg2: boolean);
	
	Init(arg0: Int, arg1: Int, arg2: boolean): void;
	
	Insert_Range(arg0: VectorRectSize, arg1: VectorRect, arg2: MaxRectsBinPack_FreeRectChoiceHeuristic): void;
	
	Insert_Single(arg0: Int, arg1: Int, arg2: MaxRectsBinPack_FreeRectChoiceHeuristic): Rect;
	
	Occupancy(): Double;
	delete(): void;
}
export interface CustomEmbindModule {
	
	IsContainedIn(arg0: Rect, arg1: Rect): boolean;
	VectorRect: typeof VectorRect
	Rect: typeof Rect
	VectorRectSize: typeof VectorRectSize
	RectSize: typeof RectSize
	MaxRectsBinPack: typeof MaxRectsBinPack
	MaxRectsBinPack_FreeRectChoiceHeuristic: typeof MaxRectsBinPack_FreeRectChoiceHeuristic
}
declare function factory(): Promise<CustomEmbindModule>;
export default factory;
